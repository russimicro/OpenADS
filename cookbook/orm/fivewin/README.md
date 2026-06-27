# ORM track -- FiveWin grid

A FiveWin **xBrowse** grid whose rows come from the **ORM**, not from a
hand-written record loop. The ORM opens a navigational DBF/CDX table through
OpenADS as a lazy `TORMCursor`; the rows feed the grid.

This complements [`../../fivewin/crud_browse.prg`](../../fivewin/), which drives
the grid with **raw xBase verbs** — same engine, one layer lower. Here you work
with `TORMConnection` / `TORMCursor` / `TORMBrowseSource` instead.

```harbour
oConn := TORMConnection():New( "dbf://" + cDir )     // navigational backend
oCur  := TORMCursor():New( oConn, "clientes" ):Open() // lazy cursor
oSrc  := TORMBrowseSource():New():FromCursor( oCur )   // single source interface
* ... read oSrc and hand the rows to an xBrowse (ARRAY mode) ...
```

> The render uses xBrowse **ARRAY mode** (the most predictable path). The ORM
> also exposes a code-block source (`ORM_BrowseBlocks`) for a fully lazy bind —
> see the ORM repo's `examples/grid_fivewin.prg`.

## Run modes

```cmd
grid_orm.exe          :: opens the window with the grid
grid_orm.exe /auto    :: no window -- reads rows through the ORM and writes
                      :: grid_auto_result.txt (head-less build smoke)
```

## Build

```cmd
set OADS_ORM_SRC=C:\path\to\hb_orm2\src
set OPENADS_INCDIR=C:\path\to\OpenADS\include
set FWDIR64=C:\path\to\FWH
build_fw.cmd  C:\path\to\folder-with-the-DLL
grid_orm.exe
```

`build_fw.cmd` reads the ORM source folder, the include folder, the FiveWin
install and the DLL folder from the environment / arguments — no baked-in paths.

> **FiveWin is a commercial product** (FiveTech Software). You need your own
> FiveWin license to build this example. The rest of the ORM track (console /
> SQL back-ends) needs only Harbour + the OpenADS engine. The companion ORM
> (**hb_orm2**, MIT) is at <https://github.com/Admnwk/hb_orm2>.

Data is invented (a tiny `clientes` list). No real records.

## Full app — multi-backend (menu / browse / CRUD)

[`app/`](app/) goes further: a complete **MDI application** driven by a menu,
running the **same code** over **SQLite / DBF / ADT / MariaDB** — the only thing
that changes is the connection URI. It shows browse (xBrowse), CRUD dialogs,
search, relations (`hasMany` / eager `belongsTo`) and soft-delete.

```cmd
set OADS_ORM_SRC=C:\path\to\hb_orm2\src
set OPENADS_INCDIR=C:\path\to\OpenADS\include
set FWDIR64=C:\path\to\FWH
app\build_app.cmd  C:\path\to\folder-with-the-DLL
app\orm_app.exe            :: GUI
app\orm_app.exe /selftest  :: head-less data smoke (no window)
```

SQLite and DBF run fully; ADT runs (creates `.adt`, with a known decimal
limitation on NUMERIC fields); MariaDB needs a running server via the
`HBORM_MARIA_URI` environment variable, otherwise it degrades gracefully.
No FiveWin sources are included here — the build only references `FWDIR64`.
