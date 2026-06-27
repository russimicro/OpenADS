# OpenADS ODBC driver

A self-contained ODBC driver DLL (`openads_odbc.dll`) that lets any ODBC
consumer — **pyodbc, PHP `PDO_ODBC`, Power BI, Excel, LibreOffice, Tableau,
DBeaver, …** — connect *to* OpenADS. It wraps the OpenADS thin SQL C API
(`openads_sql.h`); no other OpenADS DLL is needed at runtime.

## Build

```
cmake --preset msvc-x64 -DOPENADS_BUILD_ODBC_DRIVER=ON
cmake --build build/msvc-x64 --target openads_odbc --config Release
```

Output: `build/.../bindings/odbc/openads_odbc.dll`.

## Register the driver

The Driver Manager finds a driver through the registry. No machine-wide
install or admin rights are required — register it **per user** under `HKCU`.

Save as `openads_odbc.reg` (fix the path to the DLL, double backslashes), then
double-click it:

```reg
Windows Registry Editor Version 5.00

[HKEY_CURRENT_USER\SOFTWARE\ODBC\ODBCINST.INI\OpenADS]
"Driver"="C:\\path\\to\\openads_odbc.dll"
"Setup"="C:\\path\\to\\openads_odbc.dll"
"UsageCount"=dword:00000001

[HKEY_CURRENT_USER\SOFTWARE\ODBC\ODBCINST.INI\ODBC Drivers]
"OpenADS"="Installed"
```

(Machine-wide registration uses the same keys under `HKEY_LOCAL_MACHINE` and
needs admin rights.)

## Connect (DSN-less)

Connection-string keys: `DataDir` (the data directory, or `Database`) and
`ServerType` (`local` default, or `remote`).

**pyodbc**
```python
import pyodbc
cn = pyodbc.connect(r"DRIVER={OpenADS};DataDir=C:\data\mydb;ServerType=local")
cur = cn.cursor()
cur.execute("SELECT NAME, AGE FROM people")
for row in cur.fetchall():
    print(row)

# schema browsing works via the catalog functions
for t in cur.tables():        print(t.table_name)
for c in cur.columns("people"): print(c.column_name)
```

**Excel / Power BI**: *Get Data → ODBC → DSN-less* with the same
`DRIVER={OpenADS};DataDir=…` string, or create a User DSN pointing at the
`OpenADS` driver.

## What this slice covers

Connect, `SQLExecDirect` / `SQLPrepare`+`SQLExecute`, result describe + forward
fetch + `SQLGetData` (character), the catalog functions `SQLTables` /
`SQLColumns` / `SQLGetTypeInfo` / `SQLPrimaryKeys`, `SQLGetInfo`, and the
attribute accept-stubs a Driver-Manager client needs.

Not yet: typed `SQLGetData` (all columns are surfaced as character),
`SQLBindParameter` for parameterised queries, primary-key reporting, and
scrollable cursors. These are follow-up slices.
