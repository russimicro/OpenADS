@echo off
rem ===========================================================================
rem  openads-studio.bat — one-click OpenADS Studio (the ARC replacement).
rem
rem  Double-click this from the folder where openads_serverd.exe lives. It
rem  starts the engine with the web console enabled and opens your browser at
rem  the admin UI: browse tables, run SQL, manage the data dictionary — the
rem  same jobs the Advantage Data Architect (ARC) did, in the browser.
rem
rem  Optional first argument: the data directory to open (defaults to the
rem  current folder).  Example:  openads-studio.bat C:\mydata
rem
rem  The wire port is ephemeral (--port 0) so this never clashes with a
rem  running Advantage service on 6262; the Studio itself is on 6263.
rem ===========================================================================
setlocal
set "HTTP_PORT=6263"
set "DATA=%CD%"
if not "%~1"=="" set "DATA=%~1"

echo.
echo  OpenADS Studio  ->  http://127.0.0.1:%HTTP_PORT%/
echo  Data directory  :  %DATA%
echo.

start "OpenADS server" "%~dp0openads_serverd.exe" --port 0 --http-port %HTTP_PORT% --data "%DATA%"
rem give the server a moment to bind before opening the browser
ping -n 2 127.0.0.1 >nul
start "" "http://127.0.0.1:%HTTP_PORT%/"

echo  The server is running in its own window titled "OpenADS server".
echo  Close that window (or press Ctrl+C in it) to stop the database.
endlocal
