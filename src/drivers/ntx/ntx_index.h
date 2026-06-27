#pragma once

#include "drivers/index_trait.h"
#include "platform/file.h"

#include <array>
#include <string>
#include <unordered_map>
#include <vector>

namespace openads::drivers::ntx {

constexpr std::uint16_t NTX_PAGE_SIZE = 1024;
constexpr std::uint16_t NTX_MAX_KEY   = 256;

class NtxIndex final : public IIndex {
public:
    util::Result<void> open(const std::string& path, IndexOpenMode mode) override;

    std::string name()       const override { return tag_name_; }
    std::string expression() const override { return key_expr_; }
    bool        descending() const override { return descend_; }
    bool        unique()     const override { return unique_; }
    std::uint16_t key_length() const override { return key_size_; }

    util::Result<SeekOutcome> seek_first() override;
    util::Result<SeekOutcome> seek_last()  override;
    util::Result<SeekOutcome>
        seek_key(const std::string& key, bool soft) override;
    util::Result<SeekOutcome> next()       override;
    util::Result<SeekOutcome> prev()       override;
    std::string current_key() const override { return current_key_; }

    util::Result<void> insert(std::uint32_t recno,
                              const std::string& key) override;
    util::Result<void> erase (std::uint32_t recno,
                              const std::string& key) override;
    util::Result<void> flush() override;

    // Static helper used by AdsCreateIndex paths.
    static util::Result<NtxIndex>
        create(const std::string& path,
               const std::string& tag_name,
               const std::string& key_expr,
               std::uint16_t      key_size,
               bool               unique,
               bool               descend);

    using Page = std::array<std::uint8_t, NTX_PAGE_SIZE>;

private:

    struct StackFrame {
        std::uint32_t page;
        std::int32_t  key_index;
    };

    struct CachedKey {
        std::uint32_t recno;
        std::string   key;
    };

    util::Result<void> ensure_cache_();
    util::Result<void> walk_subtree_(std::uint32_t page_off,
                                     std::vector<CachedKey>& out);

    util::Result<Page*> get_page_(std::uint32_t offset);
    util::Result<void>  flush_page_(std::uint32_t offset);
    util::Result<void>  load_current_key_();

    static std::uint16_t get_key_count(const Page& p);
    static void          set_key_count(Page& p, std::uint16_t n);
    static std::uint16_t get_key_offset(const Page& p, std::int32_t i);
    static void          set_key_offset(Page& p, std::int32_t i, std::uint16_t off);
    static std::uint32_t get_left_child(const Page& p, std::int32_t i);
    static std::uint32_t get_recno     (const Page& p, std::int32_t i);
    static const std::uint8_t*
                         get_key_data  (const Page& p, std::int32_t i);
    static void          put_left_child(Page& p, std::int32_t i, std::uint32_t v);
    static void          put_recno     (Page& p, std::int32_t i, std::uint32_t v);

    util::Result<SeekOutcome> descend_leftmost_(std::uint32_t root);
    util::Result<SeekOutcome> descend_rightmost_(std::uint32_t root);
    util::Result<SeekOutcome>
        seek_key_for_write_(const std::string& padded_key, bool soft);

    platform::File                                 file_;
    IndexOpenMode                                  mode_      = IndexOpenMode::ReadOnly;
    std::uint32_t                                  root_page_ = 0;
    std::uint32_t                                  next_avail_= 0;
    std::uint16_t                                  key_size_  = 0;
    std::uint16_t                                  item_size_ = 0;
    std::uint16_t                                  max_keys_  = 0;
    std::uint16_t                                  half_page_ = 0;
    bool                                           unique_    = false;
    bool                                           descend_   = false;
    std::string                                    key_expr_;
    std::string                                    for_expr_;
    std::string                                    tag_name_;

    // Per-page cache (read-then-mutate).
    std::unordered_map<std::uint32_t, Page>        page_cache_;
    std::unordered_map<std::uint32_t, bool>        dirty_;

    std::vector<StackFrame>                        stack_;
    std::string                                    current_key_;
    std::uint32_t                                  current_recno_ = 0;

    // M3.8 cache-based traversal. The tree is walked depth-first into
    // a flat vector on first read access; nav methods then walk that
    // vector. Cache is invalidated by any insert / erase.
    std::vector<CachedKey>                         cache_;
    bool                                           cache_dirty_ = true;
    std::int64_t                                   cache_idx_   = -1;
};

} // namespace openads::drivers::ntx
