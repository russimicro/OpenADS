# OpenADS — TODO

Items are ordered by priority within each section.
Check off completed work and commit the file update so it stays current.

---

## Data Dictionary (DD) support
Data dictionaries are proving to be too hard to maintain.  We keep going back and forth.  You fix one thing, while at the same time breaking another.    I think the problem is that we are trying to maintain certain amount of compatability with SAP ADS DD.  We don't need to do that.  We can create our own structure for data dictionaries.  The important thing is to name data dictionaries with a .add file extention, memos be stored on .am file extension and any indexes as a compound .ai index.  The .add file can keep compatability with .adt, the .am with .adm and the .ai with .adi.  The only reason we name these files with alternate file extention is sto distinguish a data dictionary file with data files that will be boudn to a data dictionary.

Data dictionary needs to store db schema, functions, stored procedures, triggers, referential integrity rules, user groups (or roles), group permissions, users, direct user permissions, and user membership to groups.   User effective permissions will become user direct permissions plus permissions of groups the user belongs to.

In directory C:\Program Files (x86)\Advantage 10.10\Help you have help files you can read. Search for data dictionary to see everything a data dictionary does in SAP ADS.   We want to honor that but what's important that you understand is that we will store DD data with our own prefferd record structure.  I suggest we write records as objects.  Each object has a number, tables, stored procedures, functions, users, groups, referential integrity rules, triggers.  Each object may have a parent.  For example a trigger needs to have a parent table.  Each object has at lease a name.  For a table is the table name that doesn't necessarilty is the file name.  A lot of the data for many objects is different depending on the object type.  This data with different labels (or columns) can be stored as json structures on the memo file (.am).

The import tools we wrote (f:\OpenAds\Tools\import_dd ) will need to be ammended so that it reads from SAP DD using SAP ACE each and every DD information and writes it to the newly created imported DD using OpenAds ACE on our own OpenADs DD format.

Using SAP ACE64 you can query all DD properties (read from help files) and make sure all these properties can be stored on our newly designed OpenADS DD format.  And also make sure we have impelmented each and every SAP ACE function and stored procedres documented on help files. 

After we have implemented our new DD structure, we want to write tests where a new DD is created, functions, sp, RI, users, groups, ect... are added and stored and tested.

Once we have this working, document how dd work with OpenADs on readme.md of this project. 

Our new DD needs to keep permissions information.  Permissions are granted to groups and directly to users.  A user's effective permissions is the combination or sum of permissions granted to the group the user belongs to and the permissions directly assigned to the user.  Permissions can be assigned over many objects: tables, stored procedures, functions.  A user may have permission to execute a given function or procedure, to insert, update, delete, modify from tables with grant or without.  Permissions can also be assigned to specific fields inside tables.  This information needs to be stored on the dd in a way that is quickly retrievable.  Imagine a transaction that triggers a table trigger or selecting from multiple tables, the ADS server has to check if the user has approriate permissions before actually executing.  That is a lot of checking.  I imagine permissions information could be kept in ADS Server memroy so that it can be retrieve as fast as possible.   How do you suggest we store this information on the DD?  Perhaps have an additional integer field and store bits for each operation on or off so that it can be read quickly?  Can OpenAds load permissions into memor vars so that it can access this info without having to re-read time after time for each transaction?

### Done
- [x] Text-format DD parse + save (`# OpenADS Data Dictionary v1`):
      TABLE, INDEX, USER, MEMBER, LINK, RI, DBPROP, USERPROP rows;
      atomic write-then-rename save.
- [x] `AdsConnect60` opens a `.add` path — parent dir becomes data dir.
- [x] Full DD ABI CRUD: `AdsDDAddTable`, `AdsDDRemoveTable`,
      `AdsDDCreateUser`, `AdsDDDeleteUser`, `AdsDDAddUserToGroup`,
      `AdsDDRemoveUserFromGroup`, `AdsDDCreate{,Drop,Modify}Link`,
      `AdsDDCreate/RemoveRefIntegrity`, `AdsDDSet/GetDatabaseProperty`,
      `AdsDDGet/SetUserProperty`, `AdsDDCreate`, `AdsDDAddTable90`,
      `AdsDDCreateRefIntegrity62`.
- [x] Binary `.add` reader — `DataDict::load_add_binary_()`.
      Parses all `Table` records from real ADS proprietary files.
      Fixture: `tests/fixtures/adi/pmsys.add`. (2026-05-24)

### Open

- [x] **Binary `.add` write support** — full round-trip mutations on
      ADS proprietary Data Dictionary files. `save_add_binary_()` serializes
      all records back in exact binary layout (524-byte records, 2200-byte
      header updated in-place). `add_table`, `remove_table`, `add_index_file`,
      `remove_index_file`, `create_user`, `delete_user` all dispatch through
      the binary path when `binary_format_` is set. 4 round-trip tests added
      to `tests/unit/data_dict_test.cpp`. (2026-05-24)

- [x] **`AdsDDGetTableProperty` / `AdsDDSetTableProperty`**.
      Exported. Handles: RELATIVE_PATH (211), TABLE_PATH (205),
      TABLE_TYPE (204, inferred from extension), CHAR_TYPE (212),
      OBJ_ID (208), FIELD_COUNT (206, returns 0), and the numeric-zero
      boolean properties. Set returns AE_FUNCTION_NOT_AVAILABLE.
      5 unit tests in `tests/unit/abi_dd_table_prop_test.cpp`. (2026-05-24)

- [x] **`AdsDDSetUserProperty`** — implemented and exported.
      Property code dispatch: 1102 (GROUP_MEMBERSHIP) → `add_user_to_group`;
      1103 (BAD_LOGINS) → read-only no-op; all other codes (including
      1101 PASSWORD, 1 COMMENT) stored as `"prop_N"` string properties,
      symmetrical with `AdsDDGetUserProperty`. `AdsDDCreateUser` now
      stores password (`prop_1101`), description (`prop_1`), and optional
      group membership. 7 tests in `tests/unit/abi_dd_user_prop_test.cpp`.
      Cross-checked against `f:\harbour3.2-bcc7.3\contrib\rddads\`. (2026-05-24)

- [x] **ADI level-2 page navigation** — fixed.
      Character-key ADI indexes (CICHAR/CHAR fields) use level-2 dense-leaf
      pages instead of level-3. Branch entries use a different format:
      `padded_key[(len+3)&~3] + cum[4 LE] + page[1]` vs numeric's
      `key[8 BE] + cum[4 BE] + page[4 BE]`. Compound-key tags (e.g. "F2;F14")
      are now parsed and their total key length computed correctly.
      `is_dense_leaf()` now accepts both level-2 and level-3 pages.
      `encode_adt_key` handles CICHAR/CHAR (returns raw field bytes).
      2 new unit tests in `abi_adi_smoke_test.cpp`. (2026-05-24)

- [x] **RI enforcement at write time**.
      INSERT (AdsWriteRecord after AdsAppendRecord): validates FK exists in
      parent table via linear scan; blank FK skips the check (NULL semantics).
      DELETE (AdsDeleteRecord): enforces `delete_opt` —
      RESTRICT (2) rejects if children exist; CASCADE (1) marks child rows
      deleted; SETNULL (3) / SETDEFAULT (4) blank the child FK field.
      `pending_appends()` set tracks in-flight appends; `in_ri_check()` flag
      prevents recursive enforcement on cascade actions. UPDATE enforcement
      deferred (see below). 8 tests in `tests/unit/abi_dd_ri_test.cpp`.
      (2026-05-25)

- [x] **RI enforcement on UPDATE (parent key change)**.
      PK snapshot captured at navigation time (AdsGotoRecord/Top/Bottom/Skip/Seek)
      via `pk_snapshots()` map keyed by Table*. At AdsWriteRecord, the new PK
      (from dirty buffer) is compared to the old PK (from snapshot). If they
      differ, `update_opt` is enforced: RESTRICT rejects and restores the old
      value on disk; CASCADE updates child FK fields; SETNULL/SETDEFAULT blanks
      child FK fields. On-disk rollback on RESTRICT: `set_field` writes
      immediately so ri_enforce_update re-writes the old value when rejecting.
      4 tests in `tests/unit/abi_dd_ri_test.cpp`. (2026-05-26)

- [x] **DD authentication for local connections**.
      `AdsConnect60` checks the DD's `ADS_DD_LOG_IN_REQUIRED` property
      (`prop_5`). When set, the supplied `pucUser`/`pucPwd` are validated
      against the DD: unknown user or password mismatch both return
      `AE_LOGIN_FAILED` (7077). On success the authenticated username is
      stored on `Connection::username_` for future permission checks.
      6 tests in `tests/unit/abi_dd_auth_test.cpp`. (2026-05-25)

- [x] **Per-table access control / permission checking**.
      DD stores `TABLEPERM <table>;<user_or_group>=<level>` entries
      (0=none, 1=read, 2=write, 3=delete, 4=full). Effective level is
      max of direct user perm and any group memberships; tables with no
      ACL default to full access. `AdsOpenTable` with `usCheckRights≠0`
      enforces the level (1 for `ADS_READONLY`, 2 otherwise). SQL
      `AdsExecuteSQLDirect` with `check_rights≠0` enforces per-operation
      (SELECT→1, INSERT/UPDATE→2, DELETE→3). `AdsDDGetTableProperty` for
      property 216 returns effective level. New `AdsDDSetUserTableRights` /
      `AdsDDGetUserTableRights` manage per-user/group permissions
      programmatically. 7 tests in `tests/unit/abi_dd_perms_test.cpp`.
      (2026-05-26)

- [x] **`AdsDDGetFieldProperty` / `AdsDDSetFieldProperty`** — per-field
      metadata read/write. Structural props (name 301, type 302,
      length 303, decimals 304) read live from the table file via a
      brief open; stored props (required 305, default 306, rule 307,
      msg 308, comment 309) stored in `DataDict::field_props_` and
      persisted as `FIELDPROP` rows in the text format.
      6 tests in `tests/unit/abi_dd_field_prop_test.cpp`. (2026-05-26)

- [x] **DD triggers** — `AdsDDCreateTrigger` / `AdsDDDropTrigger` /
      `AdsDDGetTriggerProperty` / `AdsDDSetTriggerProperty`.
      Model: `TriggerEntry { name, table_alias, event_mask, container,
      procedure, priority, enabled, comment }` in `DataDict::triggers_`.
      Persisted as `TRIGGER` rows in the text format. Event mask uses
      ADS_BEFORE/AFTER_INSERT/UPDATE/DELETE bits. Execution is a no-op
      stub (trigger definition is stored and queryable; user code not
      called). 5 tests in `tests/unit/abi_dd_trigger_test.cpp`. (2026-05-26)

- [x] **DD stored procedures** — `AdsDDCreateProcedure` /
      `AdsDDDropProcedure` / `AdsDDGetProcProperty` /
      `AdsDDSetProcProperty`. Model: `ProcEntry { name, container,
      procedure, input_params, output_params, comment }` in
      `DataDict::procs_`. Persisted as `PROC` rows. Execution is a
      no-op stub. 4 tests in `tests/unit/abi_dd_proc_view_test.cpp`.
      (2026-05-26)

- [x] **DD views** — `AdsDDCreateView` / `AdsDDDropView` /
      `AdsDDGetViewProperty` / `AdsDDSetViewProperty`.
      Model: `ViewEntry { name, sql, comment }` in `DataDict::views_`.
      Persisted as `VIEW` rows. `AdsOpenTable` expansion of view alias
      → `AdsExecuteSQLDirect` deferred (see system.* SQL item below).
      5 tests in `tests/unit/abi_dd_proc_view_test.cpp`. (2026-05-26)

- [x] **`system.*` SQL virtual tables** — `SELECT * FROM system.tables`
      and 10 other aliases: `system.indexes`, `system.users`,
      `system.usergroups`, `system.permissions`, `system.relations`,
      `system.links`, `system.triggers`, `system.storedprocedures`,
      `system.views`, `system.dictionary`. Each builds an in-memory
      temp DBF from `DataDict` state and opens it as a read-only cursor.
      `AdsOpenTable` view-alias expansion: opening a DD view name
      executes the view's SQL via `AdsExecuteSQLDirect` and returns the
      cursor. Both DBF and ADT table types are reflected in
      `system.tables.TABLE_TYPE`. 9 tests in
      `tests/unit/abi_sql_system_tables_test.cpp`. (2026-05-26)

- [x] **`AdsDDGetIndexProperty` / `AdsDDSetIndexProperty`** — per-index
      metadata read from open index bindings. Properties: file name
      (401), expression (402), unique (403), descending (404),
      condition (405, returns ""), key length (406), type (407,
      returns 0). `AdsDDSetIndexProperty` returns
      AE_FUNCTION_NOT_AVAILABLE. (2026-05-26)


- [x] **DD-related SQL statements** — complete:
      `CREATE DATABASE "path" [PASSWORD ... DESCRIPTION ... ENCRYPT ...]`;
      `GRANT right [("col")] ON object TO principal`;
      `REVOKE right [("col")] ON object FROM principal`;
      17 built-in `sp_*` stored procedures via `EXECUTE PROCEDURE`:
      `sp_CreateUser/DropUser`, `sp_CreateGroup/DropGroup`,
      `sp_AddUserToGroup/RemoveUserFromGroup`,
      `sp_ModifyUserProperty/ModifyGroupProperty`,
      `sp_AddTableToDatabase/AddIndexFileToDatabase`,
      `sp_ModifyTableProperty/ModifyFieldProperty`,
      `sp_CreateReferentialIntegrity/DropReferentialIntegrity`,
      `sp_CreateLink/DropLink`, `sp_ModifyDatabase`.
      `system.iota` (1-row scalar table) and `system.columns`
      (per-field metadata) added to virtual-table set.
      `DataDict` gains explicit `GROUP` storage + `create_group`/`delete_group`.
      10 tests in `tests/unit/abi_sql_dd_sql_test.cpp`. (2026-05-26)

- [ ] **ADS proprietary ADT encryption** — out of scope for now.
      We use our own AES encryption (M11.2). ADS-original per-table
      encryption format not reversed; tables with that flag will open
      but return garbled values. Deferred indefinitely.

---

## ADT / ADM driver

### Done

- [x] **`AdsPackTable` / `AdsZapTable`** for ADT.
      `platform::File::truncate()` added (Win32 + POSIX). `AdtDriver::zap()`
      truncates the file to `hdr_len` after zeroing the record count, so
      `Table::pack()` (zap + re-append survivors) leaves no stale bytes.
      Tests in `tests/unit/abi_zap_pack_test.cpp` verify record count,
      field values, and exact file size post-operation. (2026-05-24)

### Open

---

## VFP driver

### Open

- [ ] **VFP table support** (DBF `0x30` / `0x31` / `0x32`).
      `table.cpp` rejects unknown VFP header signatures. Autoinc, V/Q types,
      and NULL-bitmap work separately; combined `0x32` (autoinc + nullable
      columns) may not parse correctly yet — see `docs/known-issues.md`.
      Needs a `VfpDriver` for `_NullFlags` and VFP autoinc when combined.

---

## SQL engine

### Open

- [x] **`CONTAINS` / `LIKE` in join-cursor and aggregate `WHERE`**.
      Both the join-cursor compile path and the aggregate `FILTER`
      compile path only handle `Cmp / AND / OR / NOT` — anything else
      returns `AE_FUNCTION_NOT_AVAILABLE`. `CONTAINS` and `LIKE` are
      the most common missing operators. Fixed in `ace_exports.cpp`:
      join-cursor compile lambda, aggregate FILTER `cf` lambda, and
      CASE WHEN `compile_cond` lambda all now support LIKE (strip
      trailing spaces + `sql_like_match`) and CONTAINS (load `.fts`
      via `Fts::load/search` before building the lambda; capture the
      hit set by `shared_ptr`). 3 new tests in
      `tests/unit/abi_sql_contains_test.cpp`. (2026-05-24)

- [x] **`CASE WHEN` conditions beyond `Cmp/AND/OR/NOT`**.
      Fixed: `compile_cond` lambda in `ace_exports.cpp` now handles
      `Kind::In` (literal list), `IsNull`/`IsNotNull`, and `Between`
      in addition to the existing `Cmp/AND/OR/NOT`. (2026-05-25)

- [x] **FTS query-time token lookup**.
      Already wired: `CONTAINS(col, expr)` is handled in all four
      SQL compile lambdas (main SELECT WHERE, join-cursor, aggregate
      FILTER, CASE WHEN). Token lookup hits the `.fts` inverted index
      at compile time and captures the record set. (2026-05-24)

---

## Encryption

### Open

- [ ] **`AdsDecryptTable` / `AdsEncryptRecord` / `AdsDecryptRecord`** — out of scope.
      We use our own encryption model (M11.2 AES). ADS-original per-record
      encryption format not reversed. Stubs return `AE_FUNCTION_NOT_AVAILABLE`.

- [ ] **Key derivation hardening**.
      `Connection::set_encryption_password` zero-pads the password to
      32 bytes (noted in `connection.cpp:510`). Should use PBKDF2 or
      Argon2 once a SHA-256 implementation is in the tree. Low urgency
      for local-use scenarios; critical before any multi-user deployment
      that stores encrypted tables long-term.

---

## Transactions

### Open

- [x] **WAL crash recovery** — end-to-end crash recovery implemented and
      tested. `Connection::open` → `recover_orphan_tx_()` reads the WAL,
      identifies transactions with no COMMIT/ABORT (orphans), writes
      before-images back for UPDATE records, and marks appended rows
      deleted for APPEND records. `LsnMap` sidecar makes recovery
      crash-safe across interrupted passes. WAL APPEND record type added
      so orphan appends are tracked persistently. 3 tests in
      `tests/unit/abi_m5x_recovery_test.cpp`. (2026-05-26)

---

## ABI completeness

### Open

- [x] **`AdsEval*Expr` family** — server-side expression evaluation
      used by Harbour/X# `ADSRDD.prg` server-side query path.
      Implemented: `AdsEvalLogicalExpr` (AOF boolean expression at
      current record via `aof::evaluate_record`), `AdsEvalNumericExpr`
      (field read or numeric literal parse), `AdsEvalStringExpr`
      (field read or string literal passthrough). (2026-05-25)

- [x] **`AdsStmt*` helpers** — per-statement table-open settings
      (table_type, lock_type, char_type, read_only, check_rights,
      disable_enc, collation, passwords) stored in SqlStatement and
      threaded into AdsExecuteSQLDirect for SELECT/UPDATE/DELETE/INSERT.
      All 9 setter functions implemented. (2026-05-25)

- [x] **`AdsRestructureTable` type conversion** — CHANGE path now
      supports C↔N, C/N→L, L→C/N, and D↔C conversions. Raw-copy
      fallback for other pairs. Test updated. (2026-05-25)

- [x] **`AdsContinue`** (LOCATE/CONTINUE loop).
      Implemented: filter-aware skip(1) on the underlying Table — since
      `Table::skip()` already walks past non-matching records when a
      filter or AOF bitmap is active, `AdsContinue` is a single-step
      forward move with `*pbFound = eof() ? 0 : 1`. Test in
      `tests/unit/abi_aof_test.cpp`. (2026-05-24)

- [x] **Table-management stubs**.
      `AdsCopyTableContent` (field-name-matched copy between two open
      tables), `AdsCloneTable` (full structural clone including deleted
      records into a temp DBF; returns new handle), and
      `AdsCopyTableStructure` (schema-only copy, 0 records). All
      implemented and tested. (2026-05-25)

- [x] **Enumeration stubs**.
      `AdsGetAllTables` enumerates all table handles owned by a connection
      (iterates `HandleRegistry` for `HandleKind::Table`, filters via
      `owns_table_ptr`). `AdsGetAllIndexes` enumerates all index handles
      bound to a table (iterates `index_bindings()`). `AdsGetFTSIndexes`
      returns 0 — FTS indexes have no persistent handles in OpenADS.
      3 tests in `tests/unit/abi_enum_test.cpp`. (2026-05-25)

---

## DA-Web (Data Architect web app — port 8080)

### Done

- [x] Connect / disconnect to Data Dictionaries and Free Tables directories.
- [x] Import SAP DD (reads SAP ACE binary .add via import_dd tool, writes
      OpenADS DD); RI parent/child tags pulled from `system.relations`.
- [x] Table browser — paged rows, sort by index tag, seek, AOF filter,
      add / edit / delete rows, inline cell editing.
- [x] Table meta editor — fields list and index list (create, delete,
      rename, change expression / unique / descending).
- [x] Table triggers — per-table trigger list, create / edit / delete,
      ACE editor for body.
- [x] SQL tab — ACE editor, F5/F9 run, Ctrl+Enter run selection,
      CSV/JSON export of results, save/load named scripts.
- [x] Stored procedures and functions — view/edit body and parameter grids.
- [x] Users — group membership grid, direct permissions grid, effective
      permissions (read-only), change password.
- [x] Groups — permissions grid, members grid.
- [x] RI Objects — view and edit existing referential integrity rules
      (parent/child table+tag, update/delete rule).
- [x] Generate SQL — DDL preview for a table's CREATE TABLE statement.
- [x] Blob / binary / picture / memo fields are non-editable in table
      browser (prevent binary data corruption on inline edit).
- [x] Tab auto-close on disconnect; Done button enabled after SAP import.

### Open

- [x] **View tab** — clicking a View node opens an editable ACE pane with
      the view's SQL; Save updates via `api/save_view.php` (setViewProperty
      prop 701/702); Drop removes via `dropView`. Backed by `api/view_body.php`.

- [x] **Link tab** — clicking a Link node opens a read-only info panel
      (alias, path, user) with a Drop button; backed by `api/link_meta.php`
      (queries `system.links`) and `api/link_ops.php` (`dropLink`).

- [x] **Create / delete user from UI** — clicking the Users category node
      opens "New User" modal (calls `sp_CreateUser`); Delete button on User
      tab calls `sp_DropUser` via `api/user_ops.php`.

- [x] **Create / delete group from UI** — clicking the Groups category node
      opens "New Group" modal (calls `sp_CreateGroup`); Delete button on
      Group tab calls `sp_DropGroup` via `api/group_ops.php`.

- [x] **Create RI rule from UI** — clicking the RI category node opens "New
      RI…" modal; on OK calls `openRiTab` which opens a blank RI form
      backed by the existing `api/save_ri.php`.

- [x] **Database properties panel** — clicking the connected DD root node
      opens a Properties tab (`dbprops`) backed by `api/db_props.php`;
      shows description, login-required toggle, and version (read-only).

- [x] **Blob / memo field viewer** — eye-icon button on blob/memo/binary
      cells; text memos show content in a modal; binary/blob fields offer a
      download link via `api/blob_data.php?download=1`.

- [x] **Create table wizard** — clicking the Tables category node opens
      "New Table…" modal (name, ADT/DBF toggle, ANSI/OEM toggle) that calls
      `sp_AddTableToDatabase` via `api/table_ops.php`; opens Fields tab after.

- [x] **Drop / remove table from DD** — "Drop Table" button on the table's
      Fields meta tab calls `sp_RemoveTableFromDatabase` via `api/table_ops.php`
      after confirmation; closes all related tabs and refreshes tree.

- [x] **Table data export from browser** — Export button (📥) on the table
      browser toolbar downloads all rows as UTF-8 CSV (respects current AOF
      filter and sort) via `api/export_table.php`.

---

## Wire protocol

### Open

- [x] **Forward-only prefetch (M12.21)** — re-implemented as a
      sequential-prefetch path negotiated via a Connect capability
      flag (PR #47, v1.0.3). 50k-record loopback scan 3.9× faster.
      Previous cursor-drift issue resolved by tying lookahead block
      to forward-Skip acks with server cursor sync. (2026-06-23)
