# OpenADS + PostgreSQL example

Drives a **PostgreSQL** table through the OpenADS **ACE API** — the same
`AdsConnect` / `AdsOpenTable` / `AdsSkip` / `AdsGetField` calls you would use
for a local DBF. PostgreSQL is reached through the **OpenADS Plus** backend
(libpq); the application code is backend-agnostic.

`main.c` connects with a `postgresql://` URI, prints every row of a `clientes`
table, and inserts one row with Append → Replace → Write.

## 1. Build the engine with PostgreSQL enabled

The PostgreSQL backend is **off by default**. Build the engine with libpq:

```powershell
cmake --preset msvc-x64 `
  -DOPENADS_WITH_POSTGRESQL=ON `
  -DOPENADS_LIBPQ_INCLUDE="C:\Program Files\PostgreSQL\16\include" `
  -DOPENADS_LIBPQ_LIBRARY="C:\Program Files\PostgreSQL\16\lib\libpq.lib"
cmake --build build/msvc-x64 --config Release
```

On Linux/macOS, point `OPENADS_LIBPQ_INCLUDE` / `OPENADS_LIBPQ_LIBRARY` at your
distro's libpq (or set the matching environment variables).

Without `-DOPENADS_WITH_POSTGRESQL=ON`, connecting with a `postgresql://` URI
returns `AE_FUNCTION_NOT_AVAILABLE` (5004).

## 2. Create the demo table

```sql
CREATE TABLE clientes (
    id    INTEGER PRIMARY KEY,
    nome  TEXT,
    saldo DOUBLE PRECISION
);
INSERT INTO clientes (id, nome, saldo) VALUES
    (1, 'Ana', 10.5), (2, 'Bob', NULL), (3, 'Cid', 0.0);
```

## 3. Build the example

```powershell
cmake -S . -B build `
  -DOPENADS_INCLUDE=../../include `
  -DOPENACE_LIB=../../build/msvc-x64/src/Release/openace64.lib
cmake --build build --config Release
```

(Use the headers and `openace` import library from your OpenADS build or
install; on Linux/macOS pass `libopenace64.so` / `.dylib`.)

## 4. Run

```powershell
# default URI: postgresql://postgres@127.0.0.1:5432/postgres
build\Release\pg_example.exe

# or pass the URI explicitly
build\Release\pg_example.exe "postgresql://postgres:secret@127.0.0.1:5432/midb"

# or via the environment
$env:OPENADS_PG_URI = "postgresql://postgres@127.0.0.1:5432/midb"
build\Release\pg_example.exe
```

At run time, `openace64.dll` and `libpq.dll` must be on `PATH` (or next to the
`.exe`).

## Connection string

Accepted schemes: `postgresql://`, `postgres://`, `pgsql://`. The URI is a
standard libpq conninfo URI:

```
postgresql://user:password@host:port/database
```

## What the backend supports

Exercised by the engine's `abi_plus_postgres_*` tests:

- **Read** — `AdsGotoTop` / `AdsSkip` / `AdsAtBOF` / `AdsAtEOF`,
  `AdsGetRecordCount`, `AdsGetField` / `AdsGetString` / `AdsGetDouble`,
  `AdsGetNumFields`.
- **Write** — `AdsAppendRecord` + `AdsSetString` / `AdsSetDouble` +
  `AdsWriteRecord`, `AdsDeleteRecord`.
- **Filter** — a Clipper-style filter is pushed down to a SQL `WHERE`.
- **Seek** — by indexed column.
- A SQL `NULL` reads back as an empty field at the ACE layer.

## From xBase (Harbour / X# `rddads`)

The same URI is used as the connection/table path through the RDD, so existing
xBase code keeps working unchanged against a PostgreSQL back end.
