#!/bin/sh
# ============================================================================
#  openads-studio.sh — one-click OpenADS Studio (the ARC replacement).
#
#  Run this from the folder where the openads_serverd binary lives. It starts
#  the engine with the web console enabled and opens your browser at the admin
#  UI: browse tables, run SQL, manage the data dictionary — the same jobs the
#  Advantage Data Architect (ARC) did, in the browser.
#
#  Optional first argument: the data directory to open (defaults to $PWD).
#      ./openads-studio.sh /var/lib/mydata
#
#  The wire port is ephemeral (--port 0) so this never clashes with a running
#  Advantage service on 6262; the Studio itself is on 6263.
# ============================================================================
set -e
DIR=$(cd "$(dirname "$0")" && pwd)
HTTP_PORT=6263
DATA=${1:-$PWD}

echo
echo " OpenADS Studio  ->  http://127.0.0.1:${HTTP_PORT}/"
echo " Data directory  :  ${DATA}"
echo

"$DIR/openads_serverd" --port 0 --http-port "$HTTP_PORT" --data "$DATA" &
SRV=$!

# give the server a moment to bind, then open the browser (best effort)
sleep 1
(xdg-open "http://127.0.0.1:${HTTP_PORT}/" 2>/dev/null \
    || open "http://127.0.0.1:${HTTP_PORT}/" 2>/dev/null \
    || true)

echo " Server PID ${SRV}. Press Ctrl+C (or 'kill ${SRV}') to stop the database."
wait "$SRV"
