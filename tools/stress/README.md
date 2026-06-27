# OpenADS stress & soak harnesses

Self-contained load tools for shaking out correctness and thread-safety bugs
that the unit suite (single-threaded, small data) does not reach. Every harness
seeds its own data under a temp directory, runs, checks an integrity oracle, and
**exits non-zero if anything is wrong** — so they drop straight into CI or a
`ctest` smoke tier. None of them need a pre-existing server, fixture, or any
machine-specific path.

## Building

The harnesses are ordinary CMake targets linked against `openads_core`:

```sh
cmake -S . -B build -DOPENADS_WITH_SQLITE=ON
cmake --build build --target openads_daily_ops openads_concurrency_stress \
                            openads_remote_random openads_remote_stress \
                            openads_stress openads_memo_stress
```

(`openads_remote_*` spin up an in-process TCP server on `127.0.0.1:0`, so no
external `openads_serverd` is required.)

## The harnesses

| Target | Mode | What it exercises |
|--------|------|-------------------|
| `openads_daily_ops` | local, in-process | **Real business-day mix** (see below) |
| `openads_concurrency_stress` | local, in-process | Concurrent append / locked read-modify-write on one shared table |
| `openads_stress` | local, in-process | Bulk write throughput (+ optional index build) |
| `openads_memo_stress` | local, in-process | Memo (`.fpt`/`.adm`) write/read |
| `openads_remote_random` | TCP wire | Randomized client mix: scan / `SELECT *` / `COUNT` / AOF / batch prefetch / reconnect churn |
| `openads_remote_stress` | TCP wire | Concurrent multi-client scan load |

### `openads_daily_ops`

A line-of-business application's day is not a pure read or write loop — it
interleaves, under concurrency:

```
SCAN        full walk of an order (every tag must reach EOF seeing all rows)
SEEK        keyed lookup of a value known to exist
FILTER/AOF  Advantage Optimized Filter set/clear (must only see matches)
ORDER SWAP  switch the active index order mid-session
SCOPE       range-limit an order, walk it (must not leak out-of-range rows)
TEMP INDEX  build an ad-hoc index for a report, use it, discard it
REINDEX     rebuild a table's indexes
APPEND      add rows
```

Each worker thread owns its own ABI connection. The seeded `CUSTOMERS` table is
shared **read-only** across threads (scan / seek / filter / order-swap / scope
are read paths). The mutating patterns (temp index / reindex / append) run on a
**private per-thread table**, so the run has a deterministic integrity oracle and
no lock contention masks a real bug. Every operation is checked: an ABI failure
bumps `errors`, a wrong result (short walk, scope leak, filter leak, lost append)
bumps `miscount`. Exit code is 0 iff both are zero.

```
openads_daily_ops [--threads C] [--seconds S] [--rows N] [--seed K] [--dir D] [--csv]
                  [--mutate]   # only the create-heavy private-table ops (isolation)
```

Escalation example (ramp the thread count and watch `errors`/`miscount` stay 0):

```sh
for t in 4 8 16 32 64 128; do
  build/tools/stress/openads_daily_ops --threads $t --seconds 6 --rows 5000 --csv
done
```

## Found by these harnesses

- **Concurrent `AdsCreateIndex61` heap corruption (local mode).** The native
  (DBF/CDX/NTX/ADI) index-create path in `src/abi/ace_exports.cpp` mutated the
  process-global `index_bindings()` / `active_binding_for()` `unordered_map`s
  with no lock, while the SQL-backend branches already took `state().mu`. Two
  connections building indexes at once — e.g. several users each creating a
  temporary report index, a common daily pattern — raced the map and corrupted
  the heap (`0xC0000374` / `0xC0000005`), reproducible from as few as 4 threads.
  Reproducer: `openads_daily_ops --mutate --threads 8`. Fix: serialize the
  native create path under the registry mutex (same lock the SQL branches use).
  Regression guard: the escalation loop above, now clean to 128 threads.
