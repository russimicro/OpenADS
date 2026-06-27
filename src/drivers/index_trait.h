#pragma once

#include "drivers/dbf_common.h"
#include "platform/file.h"
#include "util/result.h"

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace openads::drivers {

enum class IndexOpenMode { ReadOnly, Shared, Exclusive };

enum class SeekHit { Exact, AfterKey, BeforeBegin, AfterEnd };

// How key bytes are encoded on disk. Text = raw/space-padded character
// bytes (default; NTX, ADI, and character CDX). FoxNumeric = the 8-byte
// order-preserving encoding FoxPro/Harbour use for numeric and date CDX
// keys. The engine builds the bytes; the B+tree itself only ever compares
// keys as opaque bytes, so this lives at the ABI/engine boundary.
enum class KeyEncoding { Text, FoxNumeric };

struct SeekOutcome {
    SeekHit       hit          = SeekHit::AfterEnd;
    std::uint32_t recno        = 0;
    bool          positioned   = false;
};

class IIndex {
public:
    virtual ~IIndex() = default;

    virtual util::Result<void>
        open(const std::string& path, IndexOpenMode mode) = 0;

    virtual std::string name()       const = 0;
    virtual std::string expression() const = 0;
    virtual bool        descending() const = 0;
    virtual bool        unique()     const = 0;
    virtual std::uint16_t key_length() const = 0;

    virtual util::Result<SeekOutcome> seek_first()   = 0;
    virtual util::Result<SeekOutcome> seek_last()    = 0;
    virtual util::Result<SeekOutcome>
        seek_key(const std::string& key, bool soft) = 0;
    virtual util::Result<SeekOutcome> next()         = 0;
    virtual util::Result<SeekOutcome> prev()         = 0;
    // Invalidate any cached cursor state so the next next() / prev()
    // doesn't try to resume from a boundary set by an earlier walk.
    // Default no-op; CdxIndex overrides to clear its CurState.
    virtual void invalidate_cursor() {}

    virtual std::string current_key() const = 0;

    // On-disk key encoding for this index. Default Text; CdxIndex returns
    // FoxNumeric once the ABI marks a numeric/date key. The engine consults
    // this when building keys (see Table::compute_index_key_).
    virtual KeyEncoding key_encoding() const { return KeyEncoding::Text; }
    virtual void set_key_encoding(KeyEncoding) {}

    virtual util::Result<void> insert(std::uint32_t recno,
                                      const std::string& key) = 0;
    virtual util::Result<void> erase (std::uint32_t recno,
                                      const std::string& key) = 0;
    virtual util::Result<void> flush() = 0;
};

} // namespace openads::drivers
