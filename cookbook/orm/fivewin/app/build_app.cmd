@echo off
rem ===================================================================
rem  Builder for the ORM + FiveWin multi-backend demo app (orm_app.prg).
rem  Companion of grid_orm: a full MDI app (menu, browse, CRUD, search,
rem  soft-delete) running the SAME code over SQLite / DBF / ADT / MariaDB.
rem  No drive letters: everything comes from arguments / environment.
rem
rem  Usage:
rem     build_app.cmd <openads-lib-dir>
rem        <openads-lib-dir>  folder with the OpenADS DLL + import lib
rem
rem  Environment:
rem     OADS_ORM_SRC    companion ORM src\ folder (hb_orm2: github.com/Admnwk/hb_orm2)
rem     OPENADS_INCDIR  folder holding openads\ace.h
rem     FWDIR64         FiveWin 64-bit install (include\ + lib\FiveH64.lib)
rem     OPENADS_ACELIB  import-lib base name (default openace64)
rem     HB_BIN          full path to hbmk2 (optional if on PATH)
rem     MSVC_SETUP      MSVC x64 env .bat (optional if already in env)
rem
rem  Head-less smoke after building (no window):  orm_app.exe /selftest
rem  GUI:                                          orm_app.exe
rem  FiveWin is a COMMERCIAL product (FiveTech) -- you need your own license.
rem  This folder contains NO FiveWin sources; the build only references FWDIR64.
rem ===================================================================
setlocal EnableDelayedExpansion
if "%~1"=="" ( echo Usage: build_app.cmd ^<openads-lib-dir^> & exit /b 2 )
set "OPENADS_LIB=%~1"
if not defined OADS_ORM_SRC   ( echo Set OADS_ORM_SRC to the ORM src folder & exit /b 2 )
if not defined OPENADS_INCDIR ( echo Set OPENADS_INCDIR to the folder holding openads\ace.h & exit /b 2 )
if not defined FWDIR64        ( echo Set FWDIR64 to your FiveWin install & exit /b 2 )
if not defined OPENADS_ACELIB set "OPENADS_ACELIB=openace64"
set "HBMK=hbmk2"
if defined HB_BIN set "HBMK=%HB_BIN%"

pushd "%~dp0"
set "HBP=orm_app.hbp"
> "%HBP%"  echo -gui
>>"%HBP%" echo -w3
rem  the app modules (this folder)
>>"%HBP%" echo orm_app.prg
>>"%HBP%" echo app_models.prg
>>"%HBP%" echo app_backends.prg
>>"%HBP%" echo app_logic.prg
>>"%HBP%" echo app_browse.prg
>>"%HBP%" echo app_crud.prg
rem  every .prg from the ORM src (works with the full hb_orm2)
for %%P in ("%OADS_ORM_SRC%\*.prg") do >>"%HBP%" echo "%%~fP"
>>"%HBP%" echo "-I%OADS_ORM_SRC%\..\include"
>>"%HBP%" echo "-I%OPENADS_INCDIR%"
rem --- FiveWin ---
>>"%HBP%" echo "-i%FWDIR64%\include"
>>"%HBP%" echo "-L%FWDIR64%\lib"
>>"%HBP%" echo -lFiveH64
>>"%HBP%" echo -lFiveHC64
>>"%HBP%" echo xhb.hbc
>>"%HBP%" echo hbct.hbc
>>"%HBP%" echo hbwin.hbc
>>"%HBP%" echo hbmzip.hbc
>>"%HBP%" echo hbziparc.hbc
rem --- Windows system libraries FWH needs ---
>>"%HBP%" echo -lgdiplus
>>"%HBP%" echo -lole32
>>"%HBP%" echo -lOleDlg
>>"%HBP%" echo -lversion
>>"%HBP%" echo -lgdi32
>>"%HBP%" echo -luser32
>>"%HBP%" echo -lkernel32
>>"%HBP%" echo -lcomctl32
>>"%HBP%" echo -lcomdlg32
>>"%HBP%" echo -ladvapi32
>>"%HBP%" echo -lshell32
>>"%HBP%" echo -loleaut32
>>"%HBP%" echo -luuid
>>"%HBP%" echo -liphlpapi
>>"%HBP%" echo -lwsock32
>>"%HBP%" echo -lmsimg32
>>"%HBP%" echo -lpsapi
>>"%HBP%" echo -lwinmm
>>"%HBP%" echo -lws2_32
>>"%HBP%" echo -luxtheme
>>"%HBP%" echo -lshlwapi
rem --- the engine + sqlite3 (hb_orm2's direct driver) ---
>>"%HBP%" echo -lrddads
>>"%HBP%" echo "-L%OPENADS_LIB%"
>>"%HBP%" echo -l%OPENADS_ACELIB%
>>"%HBP%" echo -lsqlite3
rem --- CRT compat ---
>>"%HBP%" echo -ldflag=/NODEFAULTLIB:msvcrt
>>"%HBP%" echo -ldflag=/NODEFAULTLIB:libucrt
>>"%HBP%" echo -ldflag=/NODEFAULTLIB:libvcruntime.lib
>>"%HBP%" echo -ldflag=/FORCE:MULTIPLE
>>"%HBP%" echo -lucrt
>>"%HBP%" echo -lvcruntime
>>"%HBP%" echo -llegacy_stdio_definitions

if defined MSVC_SETUP call "%MSVC_SETUP%" >nul 2>&1
set "HBMK2="
set "HB_USER_OPTIONS="
set "PATH=%OPENADS_LIB%;%PATH%"

echo === Building orm_app.prg (ORM + FiveWin 64-bit, multi-backend) ===
"%HBMK%" "%HBP%" -oorm_app -comp=msvc64 -cpu=x86_64 -static
if errorlevel 1 ( echo BUILD FAILED & popd & exit /b 1 )

copy /y "%OPENADS_LIB%\%OPENADS_ACELIB%.dll" . >nul 2>&1
echo === Build OK : orm_app.exe ===
popd
endlocal
