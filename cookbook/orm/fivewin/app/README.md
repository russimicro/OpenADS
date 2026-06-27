# ORM track -- FiveWin multi-backend app

A complete FiveWin **MDI application** driven by the **hb_orm2** ORM, running the
**same code** over four backends. The point: *only the connection URI changes.*

## What it shows

- **Menu / Backend** -- pick `SQLite` / `DBF` / `ADT` / `MariaDB`; it connects,
  seeds and opens the Clientes browse. Unavailable backends degrade with a
  message in the status bar.
- **Browse** -- `xBrowse` (ARRAY mode) with toolbar New / Edit / Delete, a search
  box and a "show deleted" toggle (soft-delete). From Clientes, a **Pedidos**
  button opens the customer's orders (`hasMany`); the Pedidos grid shows the
  customer name via eager `belongsTo` (no N+1).
- **CRUD** -- modal dialogs with GETs + validation (`Model:Create/Save`, soft `Delete`).

Modules: `orm_app.prg` (Main/MDI/menu), `app_browse.prg`, `app_crud.prg`, and the
FiveWin-free core `app_models.prg` / `app_backends.prg` / `app_logic.prg` (reused
by the head-less `/selftest`).

## Build & run

```cmd
set OADS_ORM_SRC=C:\path\to\hb_orm2\src
set OPENADS_INCDIR=C:\path\to\OpenADS\include
set FWDIR64=C:\path\to\FWH
build_app.cmd  C:\path\to\folder-with-the-DLL
orm_app.exe            :: GUI
orm_app.exe /selftest  :: head-less data smoke (no window) -> app_selftest_result.txt
```

## Backends

| Backend  | URI                              | State |
|----------|----------------------------------|-------|
| SQLite   | `sqlite3://orm_app.db`           | full |
| DBF      | `dbf://navdata_app`              | full |
| ADT      | `dbf://` with ADT table type     | runs (creates `.adt`); NUMERIC decimals truncate -- known limitation |
| MariaDB  | `tcp://...` via `HBORM_MARIA_URI` | needs a running server; degrades gracefully otherwise |

## Notes

- Generic domain (clientes / pedidos), invented data. No real records.
- **FiveWin is a commercial product** (FiveTech). This folder contains **no
  FiveWin sources** -- `build_app.cmd` only references your local install via
  `FWDIR64`. The compiled binary may be distributed; recompiling needs your own
  FiveWin license.
- Companion ORM **hb_orm2** (MIT): <https://github.com/Admnwk/hb_orm2>.
