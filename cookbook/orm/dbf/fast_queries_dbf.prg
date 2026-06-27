/*
 * fast_queries_dbf.prg
 * ------------------------------------------------------------------
 * COOKBOOK / ORM track -- the companion Harbour ORM over OpenADS.
 * Back-end: DBF file tables (navigational; connection = a folder path).
 *
 * Level: INTERMEDIATE.
 *
 * Shows the two navigational FAST PATHS the ORM uses automatically on a
 * DBF/ADS back-end -- no special API, no flags, the same query builder
 * you already use:
 *
 *   1. Index-ordered navigation (OrdScope). When you ORDER BY a single
 *      INDEXED column, the ORM walks the table in index order instead of
 *      reading every row and sorting in memory. So ORDER BY is free, a
 *      LIMIT stops early (it never scans the whole table), and a range on
 *      the ordered column is pushed to the engine as an index scope
 *      (AdsSetScope).
 *
 *   2. Advantage Optimized Filter (AOF). A WHERE in the optimizable subset
 *      (= != < <= > >=, IN, and AND/OR trees of those) is pushed to the
 *      engine's AOF. In client/server mode the server evaluates it and only
 *      matching rows travel the wire; in local mode it runs in-process -- the
 *      same glue call, the engine routes by handle.
 *
 * Both are transparent: the result is identical to a plain scan + sort +
 * filter; these paths just do less work. This example proves the optimized
 * path actually fired by reading the ORM's instrumentation counters
 * (NavOrdCount / NavOrdLastWalked, NavAofCount / NavAofLastScanned).
 *
 * Data is invented (a small list of accounts). No real records.
 *
 * Build & run: see ../README.md (same build.cmd as the other dbf example).
 * DBF support needs no special build flag.
 * ------------------------------------------------------------------
 */

#include "hborm.ch"

CREATE CLASS Account FROM TORMModel
   METHOD TableName() INLINE "accounts"
END CLASS

PROCEDURE Main()

   LOCAL oCn, oRows, i
   LOCAL aSeed := { ;
       { 1, 850, "Ana"   }, { 2, 120, "Bruno" }, { 3, 540, "Carla" }, ;
       { 4, 300, "Davi"  }, { 5, 970, "Edu"   }, { 6, 210, "Fabio" }, ;
       { 7, 660, "Gina"  }, { 8, 430, "Hugo"  }, { 9, 780, "Iris"  }, ;
       { 10, 90, "Joao"  }, { 11, 610, "Kaue" }, { 12, 360, "Lia"  } }

   ? "OpenADS cookbook -- ORM fast queries on DBF (OrdScope + AOF)"
   ? "engine:", hbo_Version()
   ?

   /* ---- 1. connect to a data DIRECTORY ------------------------ */
   PrepareDir( cDir() )
   oCn := TORMConnection():New( cDir() )
   IF ! oCn:IsOpen()
      ? "connect failed:", hbo_LastErr()
      ErrorLevel( 1 )
      QUIT
   ENDIF
   TORMConnection_Default( oCn )

   /* ---- 2. fresh INDEXED schema + seed ------------------------ *
    * The schema builder declares an index on "balance". On a DBF back-end
    * that becomes a CDX tag, which is what enables the index-ordered path. */
   oCn:Execute( "DROP TABLE accounts" )           /* harmless if it does not exist */
   TORMSchema():New( oCn ):CreateTable( "accounts", {| t | ;
       t:Integer( "id" ), ;
       t:Integer( "balance" ), ;
       t:String( "name", 24 ), ;
       t:Index( { "balance" } ) } )

   /* seed through the Model so the CDX index is maintained on insert */
   FOR i := 1 TO Len( aSeed )
      Account():New():Create( { "id"      => aSeed[ i ][ 1 ], ;
                                "balance" => aSeed[ i ][ 2 ], ;
                                "name"    => aSeed[ i ][ 3 ] } )
   NEXT
   ? "seeded", LTrim( Str( Len( aSeed ) ) ), "accounts (balance is indexed)"
   ?

   /* ---- 3. OrdScope: ORDER BY indexed column + LIMIT ---------- *
    * Top-5 by balance. The walk follows the index, so the rows come out
    * sorted and the LIMIT stops the walk after 5 records -- it never reads
    * the other 7. */
   NavResetOrdCount()
   oRows := TORMQuery():New( oCn, "accounts" ):OrderBy( "balance", "ASC" ):Limit( 5 ):Get()
   ? "top 5 by balance (ASC, index-ordered):"
   ShowRows( oRows )
   ? "  -> ordered path fired:", iif( NavOrdCount() == 1, "yes", "no" ), ;
      "| records walked:", LTrim( Str( NavOrdLastWalked() ) ), "of", LTrim( Str( Len( aSeed ) ) ), ;
      iif( NavOrdLastWalked() < Len( aSeed ), "(stopped early)", "" )
   ?

   /* same path, descending -- highest first */
   oRows := TORMQuery():New( oCn, "accounts" ):OrderBy( "balance", "DESC" ):Limit( 3 ):Get()
   ? "top 3 by balance (DESC):"
   ShowRows( oRows )
   ?

   /* ---- 4. OrdScope range: scope bounds on the index --------- *
    * balance between 300 and 700, ordered by balance. The range is pushed
    * to the engine as an index scope; the walk visits only that key range. */
   NavResetOrdCount()
   oRows := TORMQuery():New( oCn, "accounts" ) ;
              :Where( "balance", ">=", 300 ):Where( "balance", "<=", 700 ) ;
              :OrderBy( "balance", "ASC" ):Get()
   ? "balance 300..700, ordered (index scope):"
   ShowRows( oRows )
   ? "  -> ordered path fired:", iif( NavOrdCount() == 1, "yes", "no" ), ;
      "| records walked:", LTrim( Str( NavOrdLastWalked() ) ), "of", LTrim( Str( Len( aSeed ) ) )
   ?

   /* ---- 5. AOF: WHERE pushed to the engine (no ORDER BY) ------ *
    * Without an ORDER BY the ORM uses the Advantage Optimized Filter for the
    * range; only matching rows are produced by the cursor. */
   NavResetAofCount()
   oRows := TORMQuery():New( oCn, "accounts" ) ;
              :Where( "balance", ">=", 400 ):Where( "balance", "<=", 800 ):Get()
   ? "balance 400..800 (AOF, unordered):"
   ShowRows( oRows )
   ? "  -> AOF fired:", iif( NavAofCount() == 1, "yes", "no" ), ;
      "| cursor scanned:", LTrim( Str( NavAofLastScanned() ) ), "of", LTrim( Str( Len( aSeed ) ) )
   ?

   /* ---- 6. cleanup ------------------------------------------- */
   oCn:Execute( "DROP TABLE accounts" )
   oCn:Close()

   ? ""
   ? "Done."
   RETURN

/* Data directory. Override with DEMO_DB_DIR; default is a local folder. */
STATIC FUNCTION cDir()
   LOCAL c := GetEnv( "DEMO_DB_DIR" )
   RETURN iif( Empty( c ), ".." + hb_ps() + "_demo_dbf_data", c )

STATIC PROCEDURE PrepareDir( cDir )
   IF ! hb_DirExists( cDir )
      hb_DirCreate( cDir )
   ENDIF
   RETURN

STATIC PROCEDURE ShowRows( aRows )
   LOCAL h
   FOR EACH h IN aRows
      ? "    id=" + PadR( AllTrim( hb_CStr( hb_HGetDef( h, "id", "" ) ) ), 3 ) + ;
        "  balance=" + PadL( AllTrim( hb_CStr( hb_HGetDef( h, "balance", "" ) ) ), 5 ) + ;
        "  " + AllTrim( hb_CStr( hb_HGetDef( h, "name", "" ) ) )
   NEXT
   RETURN
