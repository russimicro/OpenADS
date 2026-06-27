# ORM track

The same CRUD code, run through the **companion Harbour ORM**, against
every back-end OpenADS reaches. Instead of hand-writing record loops you
work with a `Connection`, a fluent query builder, and `Model` classes;
the builder **translates your Harbour method calls into SQL** for you.

There is one folder per back-end so each example is self-contained and
shows the exact connection string for that database -- no generic
abstraction to wade through:

| Folder | Back-end | Connection string | Needs |
|--------|----------|-------------------|-------|
| [`sqlite/`](sqlite/) | SQLite | `sqlite://./demo.db` | DLL built with `OPENADS_WITH_SQLITE` |
| [`dbf/`](dbf/) | DBF (navigational) | a folder path | always available |
| [`postgresql/`](postgresql/) | PostgreSQL | `postgresql://user:pass@host:5432/db` | `OPENADS_WITH_POSTGRESQL` + a server |
| [`mariadb/`](mariadb/) | MariaDB / MySQL | `mariadb://user:pass@host:3306/db` | `OPENADS_WITH_MARIADB` + a server |
| [`odbc/`](odbc/) | any ODBC engine | `odbc://Driver={...};...` | `OPENADS_WITH_ODBC` + a driver/DSN |
| [`complete/`](complete/) | **all of the above** | each via its URI | opens every configured back-end |
| [`advanced/`](advanced/) | SQLite | `sqlite://./demo_advanced.db` | the **next-revision** ORM sources |
| [`fivewin/`](fivewin/) | DBF (grid) | a folder path | **FiveWin 64** (commercial) — the ORM cursor in an xBrowse |

> **ADT tables?** The stable ORM's navigational open targets the DBF/CDX
> table type, so an ADT (`.adt`) example lives in the **console track**
> ([`../console/06_adt.prg`](../console/)), which selects the file type
> directly through the RDD. The ORM API itself is identical once the
> table is open.

[`complete/`](complete/) opens every back-end you have configured, runs
the full CRUD cycle on each, and finishes with an **auditable
benchmark** (timings + row counts + a content checksum so the numbers
can be re-run and verified). See [`complete/README.md`](complete/README.md).

## The two ways to query

```harbour
* Raw SQL -- you write the statement:
aRows := oCn:Query( "SELECT id, name FROM people WHERE uf = 'SP'" )

* Fluent builder -- Harbour calls, translated to SQL for you:
oQ    := TORMQuery():New( oCn, "people" ):Where( "uf", "SP" ):OrderBy( "id", "DESC" )
? oQ:ToSql()                    // SEE the generated SQL before it runs
aRows := oQ:Get()
```

`:ToSql()` returns the generated SQL string so you can learn exactly what
the builder produced before running it (`:ToAst()` gives the structured
form). The builder uses bound parameters under the hood, so values never
get concatenated into the SQL text.

## Model CRUD

```harbour
CREATE CLASS Person FROM TORMModel
   METHOD TableName() INLINE "people"
END CLASS

oP := Person():New()
oP:Create( { "id" => 4, "name" => "Davi Melo", "uf" => "MG" } )
oP := Person():New():Find( 4 )
oP:Set( "uf", "BA" ) ; oP:Save()
oP:Delete()
```

## Navigational vs SQL back-ends

- **SQL back-ends** (SQLite / PostgreSQL / MariaDB / ODBC): SQL runs in
  the engine; `WHERE` / `ORDER BY` / `LIMIT` are evaluated there.
- **Navigational back-end** (DBF): there is no SQL server, so the ORM
  walks records through the engine's cursor API. The model layer
  (Find / Save / Delete) honours the deletion flag correctly here.

The same `Model` / builder code works on both; the ORM picks the right
path from the connection string.

## Building

Each example needs the **companion ORM sources** plus an OpenADS DLL.
Use the generic `build.cmd`:

```cmd
:: from a developer command prompt, in this folder:
set OADS_ORM_SRC=C:\path\to\the\orm\src
set OPENADS_INCDIR=C:\path\to\OpenADS\include
build.cmd sqlite\crud_sqlite.prg  C:\path\to\folder-with-the-DLL
sqlite\crud_sqlite.exe
```

`build.cmd` has no baked-in paths -- it reads the ORM source folder, the
include folder and the DLL folder from the environment / arguments. Full
details and the POSIX variant are in
[`../docs/building-and-running.md`](../docs/building-and-running.md).

> **Which ORM?** The companion ORM is **hb_orm2** —
> <https://github.com/Admnwk/hb_orm2> (open-source, **MIT**). Clone it and point
> `OADS_ORM_SRC` at its `src\` folder. `build.cmd` pulls **every** `.prg` from
> that folder, so it works whether you point it at a small stable build or the
> full hb_orm2 (model + fluent builder + relations + soft-deletes + observers +
> migrations + scopes, all over the OpenADS engine). The public API used here
> (`TORMConnection` / `TORMQuery` / `TORMModel`, incl. `oQ:ToSql()`) is stable
> across revisions, so the examples carry over.
>
> hb_orm2 is a **separate project** under its own MIT license; this cookbook
> does **not** vendor it — you build each example against your local checkout.
