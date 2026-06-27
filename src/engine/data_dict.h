#pragma once

#include "util/result.h"

#include <array>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace openads::engine {

// Data Dictionary — supports two on-disk formats:
//
//   Text format (OpenADS-native, v1):
//     # OpenADS Data Dictionary v1
//     TABLE     <alias>=<relative_path>
//     INDEX     <table_alias>=<index_path>[\t<comment>]
//     USER      <name>
//     GROUP     <name>
//     MEMBER    <user>=<group>
//     LINK      <alias>=<path>[\t<user>[\t<pwd>]]
//     RI        <name>=<parent>;<child>;<tag>;<opt_update>;<opt_delete>;<fail_tbl>
//     DBPROP    <key>=<value>
//     USERPROP  <user>;<key>=<value>
//
//   Binary format (ADS / SAP ACE proprietary .add):
//     Detected by "ADS Data Dictionary\0" magic at offset 0.  Full round-trip:
//     loaded records are preserved verbatim; mutations add/delete binary records
//     in-place without reformatting the file.
//
//     Group membership is stored in two ways in the binary format:
//
//     1. Permission records (primary, SAP ACE v8+):
//          obj_type="Permission", parent_id=user, info1=group, info2=0x80000000
//          Written by AdsDDAddUserToGroup and by OpenADS for all new memberships.
//
//     2. User property-byte XOR tokens (legacy, pre-v8 or AdsDDSetUserProperty):
//          Stored in the User record's property field beyond the plen bytes.
//          Format: [uint16 N×4] [N × 4-byte tokens]
//          token[slot] = K[slot] XOR group_id_LE_4bytes
//          K[slot] is a per-database constant brute-forced from known group IDs
//          at load time.  OpenADS writes Permission records, not XOR tokens.
//
//     Both sources are unioned into memberships_ on load.

class DataDict {
public:
    DataDict() = default;

    static util::Result<DataDict> open(const std::string& path);

    // Build a fresh empty DD on disk.
    static util::Result<DataDict> create(const std::string& path);

    // ---- TABLE (M6) ------------------------------------------------------
    struct TableProps {
        std::string primary_key;    // tag name of primary key (ADS_DD_TABLE_PRIMARY_KEY=202)
        std::string default_index;  // tag name of default index (ADS_DD_TABLE_DEFAULT_INDEX=213)
        std::string comment;
    };

    util::Result<void> add_table(const std::string& alias,
                                 const std::string& relative_path);
    util::Result<void> remove_table(const std::string& alias);

    std::string resolve(const std::string& alias_or_path) const;
    bool has_alias(const std::string& alias) const noexcept {
        return tables_.find(alias) != tables_.end();
    }
    const std::unordered_map<std::string, std::string>&
        tables() const noexcept { return tables_; }

    std::string get_table_property(const std::string& alias, int prop_code) const;
    void        set_table_property(const std::string& alias, int prop_code,
                                   const std::string& value);

    // ---- INDEX (M10.1) ---------------------------------------------------
    struct IndexEntry {
        std::string table_alias;
        std::string index_path;
        std::string comment;
    };
    util::Result<void> add_index_file   (const std::string& table_alias,
                                         const std::string& index_path,
                                         const std::string& comment);
    util::Result<void> remove_index_file(const std::string& table_alias,
                                         const std::string& index_path);
    const std::vector<IndexEntry>& indexes() const noexcept { return indexes_; }

    // ---- USER + MEMBER (M10.1) ------------------------------------------
    util::Result<void> create_user(const std::string& user);
    util::Result<void> delete_user(const std::string& user);
    util::Result<void> add_user_to_group     (const std::string& user,
                                              const std::string& group);
    util::Result<void> remove_user_from_group(const std::string& user,
                                              const std::string& group);
    bool has_user (const std::string& user) const noexcept {
        return users_.find(user) != users_.end();
    }
    bool is_member_of(const std::string& user,
                      const std::string& group) const;
    const std::unordered_set<std::string>& groups_of(const std::string& user) const;
    const std::unordered_set<std::string>& users() const noexcept { return users_; }
    const std::unordered_map<std::string, std::unordered_set<std::string>>&
        memberships() const noexcept { return memberships_; }

    // ---- GROUP (M-DD-SQL) ------------------------------------------------
    util::Result<void> create_group(const std::string& group);
    util::Result<void> delete_group(const std::string& group);
    bool has_group(const std::string& group) const noexcept {
        return groups_.find(group) != groups_.end();
    }
    const std::unordered_set<std::string>& groups() const noexcept { return groups_; }

    // ---- LINK (M10.1) ----------------------------------------------------
    struct LinkEntry {
        std::string alias;
        std::string path;
        std::string user;
        std::string pwd;
    };
    util::Result<void> create_link(const std::string& alias,
                                   const std::string& path,
                                   const std::string& user,
                                   const std::string& pwd);
    util::Result<void> drop_link  (const std::string& alias);
    util::Result<void> modify_link(const std::string& alias,
                                   const std::string& path,
                                   const std::string& user,
                                   const std::string& pwd);
    const std::unordered_map<std::string, LinkEntry>&
        links() const noexcept { return links_; }

    // ---- RI rules (M10.1) ----------------------------------------------
    struct RiEntry {
        std::string name;
        std::string parent;
        std::string child;
        std::string parent_tag;
        std::string child_tag;
        std::string update_opt;
        std::string delete_opt;
        std::string fail_table;
    };
    util::Result<void> create_ri(const RiEntry& e);
    util::Result<void> remove_ri(const std::string& name);
    const std::unordered_map<std::string, RiEntry>&
        ri() const noexcept { return ri_; }
    std::unordered_map<std::string, RiEntry>&
        ri()       noexcept { return ri_; }

    // ---- DB / user properties (M10.1) ----------------------------------
    // ---- Field properties (M-DD-FIELD) ----------------------------------
    // Stored props only (required, default, rule, msg, comment).
    // Structural props (name/type/length/decimals) are read live from
    // the table file in AdsDDGetFieldProperty.
    util::Result<void> set_field_property(const std::string& table,
                                           const std::string& field,
                                           const std::string& key,
                                           const std::string& value);
    std::string get_field_property(const std::string& table,
                                    const std::string& field,
                                    const std::string& key) const;

    // ---- Triggers (M-DD-TRIGGER) ----------------------------------------
    struct TriggerEntry {
        std::string   name;
        std::string   table_alias;
        std::uint32_t event_mask = 0;  // SAP event type: 1=INSERT 2=UPDATE 3=DELETE
        std::uint32_t timing     = 0;  // SAP timing:     1=BEFORE 2=INSTEAD OF 4=AFTER
        std::string   container;
        std::string   procedure;
        std::uint32_t priority = 0;
        bool          enabled  = true;
        std::string   comment;
        // ADS_TRIGOPTIONS bitmask (1407): 0x01=WANT_VALUES 0x02=WANT_MEMOS_AND_BLOBS 0x04=NO_TRANSACTION
        // Default 0x03 = include __new/__old values + memos, use implicit transactions
        std::uint32_t options   = 0x03;
    };
    util::Result<void> create_trigger(const TriggerEntry& e);
    util::Result<void> drop_trigger  (const std::string& name);

    // Look up a trigger by "table::name" key or plain name.
    // Plain name fallback: scans for a unique match; returns nullptr if ambiguous.
    const TriggerEntry* find_trigger(const std::string& key) const noexcept {
        auto it = triggers_.find(key);
        if (it != triggers_.end()) return &it->second;
        if (key.find("::") == std::string::npos) {
            const TriggerEntry* found = nullptr;
            for (const auto& kv : triggers_) {
                auto sep = kv.first.find("::");
                if (sep != std::string::npos && kv.first.substr(sep + 2) == key) {
                    if (found) return nullptr;
                    found = &kv.second;
                }
            }
            return found;
        }
        return nullptr;
    }
    TriggerEntry* find_trigger(const std::string& key) noexcept {
        auto it = triggers_.find(key);
        if (it != triggers_.end()) return &it->second;
        if (key.find("::") == std::string::npos) {
            TriggerEntry* found = nullptr;
            for (auto& kv : triggers_) {
                auto sep = kv.first.find("::");
                if (sep != std::string::npos && kv.first.substr(sep + 2) == key) {
                    if (found) return nullptr;
                    found = &kv.second;
                }
            }
            return found;
        }
        return nullptr;
    }
    bool has_trigger(const std::string& key) const noexcept {
        return find_trigger(key) != nullptr;
    }
    const std::unordered_map<std::string, TriggerEntry>&
        triggers() const noexcept { return triggers_; }
    std::unordered_map<std::string, TriggerEntry>&
        triggers()       noexcept { return triggers_; }

    // ---- Stored procedures (M-DD-PROC) ----------------------------------
    struct ProcEntry {
        std::string name;
        std::string container;
        std::string procedure;
        std::string input_params;
        std::string output_params;
        std::string comment;
    };
    util::Result<void> create_proc(const ProcEntry& e);
    util::Result<void> drop_proc  (const std::string& name);
    bool has_proc(const std::string& name) const noexcept {
        return procs_.find(name) != procs_.end();
    }
    const std::unordered_map<std::string, ProcEntry>&
        procs() const noexcept { return procs_; }
    std::unordered_map<std::string, ProcEntry>&
        procs()       noexcept { return procs_; }

    // ---- User-defined functions (ADS binary "Function" type) ---------------
    struct FunctionEntry {
        std::string name;
        std::string container;
        std::string implementation;  // SQL body of the function
        std::string input_params;
        std::string return_type;
        std::string comment;
    };
    util::Result<void> create_function(const FunctionEntry& e);
    util::Result<void> drop_function  (const std::string& name);
    bool has_function(const std::string& name) const noexcept {
        return functions_.find(name) != functions_.end();
    }
    const std::unordered_map<std::string, FunctionEntry>&
        functions() const noexcept { return functions_; }
    std::unordered_map<std::string, FunctionEntry>&
        functions()       noexcept { return functions_; }

    // ---- Views (M-DD-VIEW) ----------------------------------------------
    struct ViewEntry {
        std::string name;
        std::string sql;
        std::string comment;
    };
    util::Result<void> create_view(const ViewEntry& e);
    util::Result<void> drop_view  (const std::string& name);
    bool has_view(const std::string& name) const noexcept {
        return views_.find(name) != views_.end();
    }
    const std::unordered_map<std::string, ViewEntry>&
        views() const noexcept { return views_; }
    std::unordered_map<std::string, ViewEntry>&
        views()       noexcept { return views_; }

    // ---- Permissions (M-ACL) -----------------------------------------------
    // SAP binary info2 bitmask encoding:
    //   bit 0  = SELECT    bit 1  = UPDATE    bit 2  = INSERT   bit 3  = DELETE
    //   bit 4  = EXECUTE   bit 5  = ACCESS    bit 6  = CREATE
    //   bit 7  = ALTER     bit 8  = DROP      bit 31 = INHERIT
    // SAP object-type codes: 1=Table 3=Database 8=User 10=StoredProc 18=Function
    struct PermissionEntry {
        std::string  object_name;
        std::string  object_type;    // "Table", "StoredProc", "Function", …
        int          object_type_code = 0;
        std::string  grantee;        // user or group name
        bool         grantee_is_group = false;
        uint32_t     bitmask = 0;    // raw info2 from binary record
    };

    // Per-operation effective permissions for a principal on one object.
    // "No ACL defined for this object" → all ops allowed (open = true).
    struct EffectiveOps {
        bool select_  = false;
        bool update_  = false;
        bool insert_  = false;
        bool delete_  = false;
        bool execute_ = false;
        bool open     = false;  // true when no ACL entry exists → full access
        bool any() const noexcept {
            return open || select_ || update_ || insert_ || delete_ || execute_;
        }
    };

    // Compute effective per-operation permissions for username on a specific
    // object (any type: Table, StoredProc, Function, …).  Takes direct user
    // entries + contributions from every group the user belongs to.
    EffectiveOps get_effective_ops(const std::string& username,
                                    const std::string& object_name) const;

    // True when the DD defines at least one ACL entry for any object.
    // If false, all access is open and permission checks are skipped.
    bool has_any_acl() const noexcept { return !permissions_.empty(); }

    // Compute effective permissions for every object a username has access to
    // (used to populate system.effectivepermissions).
    struct EffectivePermEntry {
        std::string object_name;
        std::string object_type;
        int         object_type_code = 0;
        std::string grantee;
        EffectiveOps ops;
    };
    std::vector<EffectivePermEntry>
        get_all_effective_perms(const std::string& username) const;

    // Fine-grained permission grant using the SAP ADS_PERMISSION_* bitmask.
    // obj_type: "Table", "View", "StoredProc", "Function", "Database", …
    // grantee:  user or group name.
    // bitmask:  ADS_PERMISSION_* bits (0x001=SELECT, 0x002=UPDATE, 0x010=INSERT,
    //           0x020=DELETE, 0x004=EXECUTE, 0x80000000=full/sentinel).
    // Deactivates any existing SAP-written record for the same (grantee, object)
    // pair so imported bitmasks are not shadowed by the 0x80000000 sentinel.
    util::Result<void> grant_permission(const std::string& obj_type,
                                        const std::string& obj_name,
                                        const std::string& grantee,
                                        uint32_t bitmask);

    // level: 0=none, 1=read, 2=write, 3=delete, 4=full.
    // user_or_group may be a user name or a group name.
    // Thin wrapper around grant_permission("Table", …) for backward compat.
    util::Result<void> set_table_permission(const std::string& table,
                                             const std::string& user_or_group,
                                             int level);
    // Legacy coarse-grained effective level (0-4) kept for backward compat.
    int get_effective_permission(const std::string& username,
                                  const std::string& table) const;
    bool has_table_acl(const std::string& table) const noexcept {
        return table_perms_.find(table) != table_perms_.end();
    }
    // True when the DD was created by SAP ACE and contains unimported ACL
    // Permission records.  Clients should prompt the user to run the import
    // tool; AdsConnect60 surfaces this as AE_SAP_PERMS_NEED_IMPORT (5174).
    bool has_sap_permissions() const noexcept { return has_sap_permissions_; }
    // Deactivate all SAP-written Permission records (prop_null=true) and clear
    // the flag.  Called by openads_import_dd after writing imported permissions
    // so the dest file no longer triggers AE_SAP_PERMS_NEED_IMPORT (5174).
    util::Result<void> clear_sap_permissions();
    const std::unordered_map<std::string, std::unordered_map<std::string, int>>&
        table_perms() const noexcept { return table_perms_; }

    const std::vector<PermissionEntry>&
        permissions() const noexcept { return permissions_; }

    using FieldPropsMap = std::unordered_map<std::string,
                              std::unordered_map<std::string,
                                  std::unordered_map<std::string, std::string>>>;
    const FieldPropsMap& field_props() const noexcept { return field_props_; }

    // ---- DB / user properties (M10.1) ----------------------------------
    util::Result<void> set_db_property(const std::string& key,
                                       const std::string& value);
    std::string get_db_property(const std::string& key) const;
    const std::unordered_map<std::string, std::string>&
        db_props() const noexcept { return db_props_; }
    util::Result<void> set_user_property(const std::string& user,
                                         const std::string& key,
                                         const std::string& value);
    std::string get_user_property(const std::string& user,
                                  const std::string& key) const;

    util::Result<void> save();

    const std::string& path() const noexcept { return path_; }

private:
    // In-memory representation of one binary .add record.
    struct BinaryRecord {
        bool active = true;
        std::uint32_t obj_id = 0;
        std::uint32_t parent_id = 0;
        std::string obj_type;           // up to 10 chars (trimmed)
        std::string obj_name;           // up to 200 chars (trimmed)
        std::string property;           // raw VarChar bytes (may include \0)
        bool prop_null = true;          // true → stored as 0xFFFF
        // When nonzero, overrides the plen field written to disk (used for Function lstr
        // format where plen = preamble size, not full property size).
        std::uint16_t prop_plen = 0;
        std::array<std::uint8_t, 9> more_property{};
        std::uint32_t info1 = 0;
        std::uint32_t info2 = 0;
        std::array<std::uint8_t, 9> comment{};
    };

    util::Result<void> load_();
    util::Result<void> load_add_binary_(const std::string& buf);
    util::Result<void> save_add_binary_();

    std::uint32_t binary_alloc_id_();           // consume + advance next ObjID
    std::uint32_t binary_obj_id_of_(const std::string& obj_type,
                                    const std::string& name) const;
    static std::string serialize_binary_rec_(const BinaryRecord& r,
                                             std::uint32_t rec_len);

    std::string                                  path_;
    std::unordered_map<std::string, std::string> tables_;
    std::unordered_map<std::string, TableProps>  table_props_;
    std::vector<IndexEntry>                      indexes_;
    std::unordered_set<std::string>              users_;
    std::unordered_set<std::string>              groups_;
    // user → set<group>
    std::unordered_map<std::string,
                       std::unordered_set<std::string>> memberships_;
    std::unordered_map<std::string, LinkEntry>   links_;
    std::unordered_map<std::string, RiEntry>     ri_;
    std::unordered_map<std::string, std::string> db_props_;
    // user → key → value
    std::unordered_map<std::string,
                       std::unordered_map<std::string, std::string>>
                                                 user_props_;
    // table → user_or_group → level (0=none … 4=full) — for get_effective_permission()
    std::unordered_map<std::string,
                       std::unordered_map<std::string, int>>
                                                 table_perms_;
    // All permission entries from binary records (Table, StoredProc, Function, …)
    std::vector<PermissionEntry>                 permissions_;
    // table_alias → field_name → key → value (stored field props)
    std::unordered_map<std::string,
                       std::unordered_map<std::string,
                                          std::unordered_map<std::string, std::string>>>
                                                 field_props_;
    std::unordered_map<std::string, TriggerEntry>  triggers_;
    std::unordered_map<std::string, ProcEntry>     procs_;
    std::unordered_map<std::string, FunctionEntry> functions_;
    std::unordered_map<std::string, ViewEntry>     views_;

    // Binary format state (populated only when binary_format_ == true).
    bool binary_format_ = false;
    // True when the binary DD contains SAP-written ACL Permission records
    // (info2=0x80000000, prop_null=true) that have not yet been imported via
    // the openads_import_dd tool.  Set during load; cleared by grant_permission.
    bool has_sap_permissions_ = false;
    std::string binary_hdr_;            // raw hdr_len bytes, updated in-place
    std::uint32_t binary_hdr_len_ = 0;
    std::uint32_t binary_rec_len_ = 0;
    std::vector<BinaryRecord> binary_recs_;  // all records, active + deleted
};

} // namespace openads::engine
