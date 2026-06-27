#include "engine/lock_mgr.h"

namespace openads::engine {

namespace {

constexpr std::uint64_t NTX_FILE_BASE = 1'000'000'000ULL;
constexpr std::uint64_t NTX_REC_BASE  = 1'000'000'001ULL;
constexpr std::uint64_t CDX_FILE_BASE = 0x7FFFFFFEULL;
constexpr std::uint64_t VFP_FILE_BASE = 0x3FFFFFFEULL;
constexpr std::uint64_t ADT_FILE_BASE = 0x8000000000000000ULL;
constexpr std::uint64_t ADT_FILE_LEN  = 0x10000ULL;

} // namespace

std::uint64_t LockMgr::file_lock_offset(TableTypeForLock t, LockingMode m) {
    (void)m;
    switch (t) {
        case TableTypeForLock::Ntx: return NTX_FILE_BASE;
        case TableTypeForLock::Cdx: return CDX_FILE_BASE;
        case TableTypeForLock::Vfp: return VFP_FILE_BASE;
        case TableTypeForLock::Adt: return ADT_FILE_BASE;
    }
    return NTX_FILE_BASE;
}

std::uint64_t LockMgr::record_lock_offset(TableTypeForLock t, LockingMode m,
                                          std::uint32_t recno) {
    (void)m;
    switch (t) {
        case TableTypeForLock::Ntx: return NTX_REC_BASE  + recno;
        case TableTypeForLock::Cdx: return CDX_FILE_BASE - recno;
        case TableTypeForLock::Vfp: return VFP_FILE_BASE - recno;
        case TableTypeForLock::Adt: return ADT_FILE_BASE +
                                           (static_cast<std::uint64_t>(recno) << 16);
    }
    return NTX_REC_BASE + recno;
}

util::Result<LockHandle>
LockMgr::lock_table_excl(platform::File& f, TableTypeForLock t, LockingMode m) {
    std::uint64_t off = file_lock_offset(t, m);
    std::uint64_t len = (t == TableTypeForLock::Adt) ? ADT_FILE_LEN : 1ULL;
    Key k{&f, off};
    auto it = held_.find(k);
    if (it != held_.end()) {
        ++it->second;
        return LockHandle{platform::ByteLock{}, off, len};
    }
    auto bl = platform::ByteLock::acquire(f, off, len, platform::LockKind::Exclusive);
    if (!bl) return bl.error();
    held_[k] = 1;
    return LockHandle{std::move(bl).value(), off, len};
}

util::Result<LockHandle>
LockMgr::lock_record_excl(platform::File& f, TableTypeForLock t, LockingMode m,
                          std::uint32_t recno) {
    std::uint64_t off = record_lock_offset(t, m, recno);
    Key k{&f, off};
    auto it = held_.find(k);
    if (it != held_.end()) {
        ++it->second;
        return LockHandle{platform::ByteLock{}, off, 1};
    }
    auto bl = platform::ByteLock::acquire(f, off, 1, platform::LockKind::Exclusive);
    if (!bl) return bl.error();
    held_[k] = 1;
    return LockHandle{std::move(bl).value(), off, 1};
}

util::Result<LockHandle>
LockMgr::lock_record_shared(platform::File& f, TableTypeForLock t, LockingMode m,
                            std::uint32_t recno) {
    std::uint64_t off = record_lock_offset(t, m, recno);
    Key k{&f, off};
    auto it = held_.find(k);
    if (it != held_.end()) {
        ++it->second;
        return LockHandle{platform::ByteLock{}, off, 1};
    }
    auto bl = platform::ByteLock::acquire(f, off, 1, platform::LockKind::Shared);
    if (!bl) return bl.error();
    held_[k] = 1;
    return LockHandle{std::move(bl).value(), off, 1};
}

util::Result<LockHandle>
LockMgr::try_lock_table_excl(platform::File& f, TableTypeForLock t, LockingMode m) {
    std::uint64_t off = file_lock_offset(t, m);
    std::uint64_t len = (t == TableTypeForLock::Adt) ? ADT_FILE_LEN : 1ULL;
    Key k{&f, off};
    auto it = held_.find(k);
    if (it != held_.end()) {
        ++it->second;
        return LockHandle{platform::ByteLock{}, off, len};
    }
    auto bl = platform::ByteLock::try_acquire(f, off, len,
                                              platform::LockKind::Exclusive);
    if (!bl) return bl.error();
    held_[k] = 1;
    return LockHandle{std::move(bl).value(), off, len};
}

util::Result<LockHandle>
LockMgr::try_lock_record_excl(platform::File& f, TableTypeForLock t, LockingMode m,
                              std::uint32_t recno) {
    std::uint64_t off = record_lock_offset(t, m, recno);
    Key k{&f, off};
    auto it = held_.find(k);
    if (it != held_.end()) {
        ++it->second;
        return LockHandle{platform::ByteLock{}, off, 1};
    }
    auto bl = platform::ByteLock::try_acquire(f, off, 1,
                                              platform::LockKind::Exclusive);
    if (!bl) return bl.error();
    held_[k] = 1;
    return LockHandle{std::move(bl).value(), off, 1};
}

bool LockMgr::unlock_table(platform::File& f, TableTypeForLock t, LockingMode m) {
    Key k{&f, file_lock_offset(t, m)};
    auto it = held_.find(k);
    if (it == held_.end()) return false;
    if (--it->second <= 0) {
        held_.erase(it);
        return true;
    }
    return false;
}

bool LockMgr::unlock_record(platform::File& f, TableTypeForLock t, LockingMode m,
                            std::uint32_t recno) {
    Key k{&f, record_lock_offset(t, m, recno)};
    auto it = held_.find(k);
    if (it == held_.end()) return false;
    if (--it->second <= 0) {
        held_.erase(it);
        return true;
    }
    return false;
}

} // namespace openads::engine
