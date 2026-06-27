/*
 * 08_postgresql.prg
 * ------------------------------------------------------------------
 * COOKBOOK / console track -- pure Harbour against OpenADS, NO ORM.
 *
 * Level: INTERMEDIATE.
 *
 * What it shows, end to end:
 *   1. Point the engine at a PostgreSQL server with a `postgresql://`
 *      connection URI (the OpenADS Plus backend, libpq).
 *   2. Open a SERVER-side table by name and read it with the ordinary
 *      xBase verbs -- dbGoTop / dbSkip / FieldGet.
 *   3. Insert a row navigationally -- dbAppend + FIELD:= + dbCommit --
 *      which the backend turns into a SQL INSERT.
 *
 * A PostgreSQL table is just another work area here: once connected,
 * USE / dbSkip / dbAppend behave exactly like the local DBF examples
 * (01-04). Only the connection string changes.
 *
 * ---- Prerequisites ----------------------------------------------
 *   * An OpenADS DLL built with OPENADS_WITH_POSTGRESQL (libpq).
 *     Without it, AdsConnect on a postgresql:// URI returns
 *     AE_FUNCTION_NOT_AVAILABLE.
 *   * A reachable PostgreSQL server and a `people` table:
 *        CREATE TABLE people ( id INTEGER PRIMARY KEY,
 *                              name VARCHAR(40),
 *                              uf   CHAR(2) );
 *        INSERT INTO people (id, name, uf) VALUES
 *            (1, 'Ana Souza',  'SP'),
 *            (2, 'Bruno Lima', 'RJ'),
 *            (3, 'Carla Reis', 'SP');
 *
 * ---- Running it -------------------------------------------------
 * Never hardcode credentials -- pass the URI in the environment:
 *      set DEMO_PG_URI=postgresql://app:secret@127.0.0.1:5432/demo  (Windows)
 *      export DEMO_PG_URI=postgresql://app:secret@127.0.0.1:5432/demo (POSIX)
 *      08_postgresql.exe
 * With DEMO_PG_URI unset, or if the server cannot be reached, it
 * prints what to do and exits cleanly (so it never hangs in a build).
 *
 * Data is 100% invented. No real-world data.
 *
 * Build & run: see this folder's README.md, or run build.cmd.
 * ------------------------------------------------------------------
 */

#include "ads.ch"
#include "rddsys.ch"

REQUEST ADS, ADSCDX, ADSNTX

PROCEDURE Main()

   LOCAL cUri   := AllTrim( GetEnv( "DEMO_PG_URI" ) )
   LOCAL cTable := "people"
   LOCAL hConn  := 0
   LOCAL n      := 0

   ? "OpenADS cookbook -- 08 PostgreSQL (pure Harbour, OpenADS Plus)"
   ? "Engine version reported by the DLL:", AdsVersion()
   ?

   IF Empty( cUri )
      ? "Set DEMO_PG_URI to a PostgreSQL connection string, e.g.:"
      ? "    set DEMO_PG_URI=postgresql://app:secret@127.0.0.1:5432/demo"
      ? "(skipped -- nothing to connect to)"
      RETURN
   ENDIF

   ? "Connecting to:", cUri
   ?

   /* ---- 1. ask for a server connection, then open the URI ------- */
   AdsSetServerType( ADS_REMOTE_SERVER )
   RddSetDefault( "ADSCDX" )

   /* AdsConnect60( path, serverTypes, user, pass, options, @hConn ).
    * The credentials live in the URI; pass empty user/pass here. */
   IF ! AdsConnect60( cUri, ADS_REMOTE_SERVER, "", "", , @hConn ) .OR. hConn == 0
      ? "Could not connect to", cUri
      ?
      ? "Check that:"
      ? "  * the OpenADS DLL was built with OPENADS_WITH_POSTGRESQL"
      ? "  * the PostgreSQL server is reachable and the database exists"
      ? "  * libpq (libpq.dll) is on PATH at run time"
      ErrorLevel( 1 )
      QUIT
   ENDIF

   ? "Connected. Default connection handle:", AdsConnection()
   ?

   /* ---- 2. open the server-side table and read it -------------- */
   /* No path on the client: the name resolves to a table in the PG
    * database. Otherwise it is an ordinary work area. */
   USE ( cTable ) VIA "ADSCDX" NEW SHARED
   IF ! Used()
      ? "Could not open table '" + cTable + "'."
      ? "Create it first (see the header of this file for the SQL)."
      AdsDisconnect()
      ErrorLevel( 1 )
      QUIT
   ENDIF

   ? "Rows in '" + cTable + "' before insert:", LastRec()
   ? "Current rows:"
   dbGoTop()
   DO WHILE ! Eof() .AND. n < 20
      n++
      ? "  " + Str( n, 2 ) + "  id=" + ;
        AllTrim( hb_CStr( FIELD->ID ) ) + "  " + ;
        PadR( AllTrim( hb_CStr( FIELD->NAME ) ), 12 ) + ;
        "  uf=" + AllTrim( hb_CStr( FIELD->UF ) )
      dbSkip()
   ENDDO
   ?

   /* ---- 3. insert a row the navigational way ------------------- */
   /* dbAppend + FIELD assignment + dbCommit becomes a SQL INSERT on
    * the PostgreSQL side. Pick an id unlikely to collide for the demo. */
   ? "Inserting one row with dbAppend / FIELD:= / dbCommit ..."
   dbAppend()
   FIELD->ID   := 99
   FIELD->NAME := "Davi Melo"
   FIELD->UF   := "MG"
   dbCommit()

   /* Re-read so the new row is visible. */
   dbGoBottom()
   ? "Last row now:", ;
     "id=" + AllTrim( hb_CStr( FIELD->ID ) ), ;
     AllTrim( hb_CStr( FIELD->NAME ) ), ;
     "uf=" + AllTrim( hb_CStr( FIELD->UF ) )
   ?

   /* ---- 4. clean up -------------------------------------------- */
   dbCloseArea()
   AdsDisconnect()

   ? "Done."
   RETURN
