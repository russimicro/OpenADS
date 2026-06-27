# OpenADS examples

GUI / application-level showcases that point an existing xBase stack at
OpenADS' ACE DLL (`ace64.dll` / `ace32.dll`). Unlike `tests/smoke/`,
these are **not** meant to run headless in CI — they're "swap your
`ace64.dll`, run your app, see it work" demos.

| Subdir | Stack | Notes |
|--------|-------|-------|
| `adt-native/` | Harbour console + `hbmk2` | Native `.adt` / `.adi` / `.adm` create, append, index seek — **by glokcode**. |
| `harbour-hbmk2/` | Harbour console + `hbmk2` (`.hbp`) | Turnkey `.hbp` template — drop in a `.prg`, set `OPENADS_LIB`, run `hbmk2`. x64 + x86 variants, Windows + POSIX wrappers. |
| `fivewin/` | FiveWin (FWH) + Harbour | Commercial GUI lib — not vendored; install your own FWH. |
| `xsharp-winforms/` | X# + WinForms | X# runtime not vendored; install from xsharp.eu. |
| `postgresql/` | C + ACE API + CMake | OpenADS Plus PostgreSQL backend — connect by `postgresql://` URI, read + insert through the ACE API. Needs `-DOPENADS_WITH_POSTGRESQL=ON`. |

Rules: only first-party source here; never check in third-party
runtimes/libraries (FWH, X#, Harbour) or SAP-shipped binaries. Each
subdir's `README.md` says exactly how to point the app at OpenADS'
DLL.
