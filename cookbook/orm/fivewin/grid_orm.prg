/*
 * grid_orm.prg
 * ------------------------------------------------------------------
 * COOKBOOK / ORM track -- a FiveWin grid fed by the companion ORM.
 *
 * Level: INTERMEDIATE.
 *
 * The sibling ../../fivewin/crud_browse.prg drives the grid with RAW xBase
 * verbs. THIS one shows the same idea one layer up: the data comes from the
 * ORM (a TORMCursor reading a navigational DBF/CDX table through OpenADS),
 * and feeds a FiveWin xBrowse. No hand-written record loop.
 *
 * Two run modes (so the example is also a head-less build smoke):
 *   grid_orm.exe          opens the window with the grid.
 *   grid_orm.exe /auto     no window: reads the rows through the ORM and
 *                          writes grid_auto_result.txt (CI-friendly).
 *
 * Data is invented (a tiny "clientes" list). No real records.
 *
 * Build & run: see ./README.md (FiveWin 64 + OpenADS DLL + the ORM sources).
 * ------------------------------------------------------------------
 */
#include "FiveWin.ch"
#include "xbrowse.ch"
#include "hborm.ch"

/* FWH's xBrowse builds ADS row-position blocks as MACRO strings referencing
   AdsKeyNo()/AdsKeyCount(); keep those symbols in the image. */
REQUEST ADSCDX, ADSNTX
REQUEST ADSKEYNO, ADSKEYCOUNT, ADSGETRELKEYPOS, ADSSETRELKEYPOS

#define GRID_DIR  "navtmp_grid_orm"

CREATE CLASS TCliente FROM TORMModel
   METHOD TableName() INLINE "clientes"
   METHOD Casts()     INLINE { "limite" => "decimal:2" }
END CLASS

/* ================================================================= */

FUNCTION Main( cMode )
   LOCAL lAuto := ( ValType( cMode ) == "C" .AND. Lower( AllTrim( cMode ) ) == "/auto" )
   LOCAL oConn, oCur, oSrc

   hb_DirCreate( GRID_DIR )
   oConn := TORMConnection():New( "dbf://" + GRID_DIR )    // navigational backend
   IF ! oConn:IsOpen()
      ? "connect failed: dbf://" + GRID_DIR
      RETURN 1
   ENDIF

   Seed( oConn )

   IF lAuto
      RETURN AutoGrid( oConn )                             // head-less smoke
   ENDIF

   oCur := TORMCursor():New( oConn, "clientes" ):Open()    // lazy cursor
   oSrc := TORMBrowseSource():New():FromCursor( oCur )
   ShowGrid( oSrc )
   oCur:Close()
   oConn:Close()
   RETURN 0

/* ---- seed: rebuild the DBF table from scratch (idempotent) ------- */

STATIC PROCEDURE Seed( oConn )
   LOCAL aData, a
   FErase( GRID_DIR + "\clientes.dbf" )
   FErase( GRID_DIR + "\clientes.cdx" )
   TORMSchema():New( oConn ):CreateTable( "clientes", {| t | ;
      t:Id(), ;
      t:String( "nome",   30 ), ;
      t:String( "cidade", 20 ), ;
      t:Decimal( "limite", 12, 2 ) } )
   aData := { ;
      { "Maria Silva",     "Sao Paulo",      5000.00 }, ;
      { "John Doe",        "Rio de Janeiro", 3200.50 }, ;
      { "Ana Pereira",     "Belo Horizonte", 8750.00 }, ;
      { "Carlos Souza",    "Curitiba",       1500.00 }, ;
      { "Beatriz Lima",    "Porto Alegre",   9900.90 }, ;
      { "Diego Fernandez", "Salvador",       4100.00 } }
   FOR EACH a IN aData
      NavInsert( oConn, "clientes", ;
         { "nome" => a[ 1 ], "cidade" => a[ 2 ], "limite" => a[ 3 ] }, "id" )
   NEXT
   RETURN

/* ---- /auto: exercise the cursor head-less, write the result ------ */

STATIC FUNCTION AutoGrid( oConn )
   LOCAL oCur, oSrc, h, nCount, nCols, cFirst, cMsg
   oCur   := TORMCursor():New( oConn, "clientes" ):Open()
   oSrc   := TORMBrowseSource():New():FromCursor( oCur )
   h      := ORM_BrowseBlocks( oSrc )
   Eval( h[ "gotop" ] )
   nCount := Eval( h[ "count" ] )
   nCols  := Len( oSrc:Columns() )
   cFirst := AllTrim( hb_CStr( oSrc:Value( "nome" ) ) )
   cMsg := "OK: ORM grid /auto -- cursor over a navigational DBF" + hb_eol() + ;
           "  rows    = " + hb_CStr( nCount ) + hb_eol() + ;
           "  columns = " + hb_CStr( nCols )  + hb_eol() + ;
           "  first row (nome) = " + cFirst   + hb_eol()
   hb_MemoWrit( "grid_auto_result.txt", cMsg )
   ? cMsg
   oCur:Close()
   oConn:Close()
   RETURN iif( nCount == 6 .AND. nCols >= 4 .AND. cFirst == "Maria Silva", 0, 1 )

/* ---- GUI: read rows through the ORM, show them in an xBrowse ----- */

STATIC PROCEDURE ShowGrid( oSrc )
   LOCAL oWnd, oBrw, aData, aHead, i
   aData := RowsFromSource( oSrc )
   aHead := { "Id", "Nome", "Cidade", "Limite" }
   DEFINE WINDOW oWnd FROM 1, 1 TO 28, 100 ;
      TITLE "OpenADS cookbook -- ORM cursor in a FiveWin grid"
   @ 0, 0 XBROWSE oBrw OF oWnd ;
      ARRAY aData AUTOCOLS ;
      CELL LINES NOBORDER FOOTERS
   FOR i := 1 TO Len( aHead )
      IF i <= Len( oBrw:aCols )
         oBrw:aCols[ i ]:cHeader := aHead[ i ]
      ENDIF
   NEXT
   oBrw:CreateFromCode()
   oWnd:oClient := oBrw
   ACTIVATE WINDOW oWnd ;
      ON INIT   ( oBrw:SetFocus(), oBrw:GoTop(), oBrw:Refresh() ) ;
      ON RESIZE ( oBrw:adjust(), oBrw:Refresh() )
   RETURN

/* The ORM reads the rows; xBrowse ARRAY mode is the robust render path. */
STATIC FUNCTION RowsFromSource( oSrc )
   LOCAL aData := {}, xLim
   oSrc:GoTop()
   DO WHILE ! oSrc:Eof()
      xLim := oSrc:Value( "limite" )
      AAdd( aData, { ;
         hb_CStr( oSrc:Value( "id" ) ), ;
         AllTrim( hb_CStr( oSrc:Value( "nome" ) ) ), ;
         AllTrim( hb_CStr( oSrc:Value( "cidade" ) ) ), ;
         iif( ValType( xLim ) == "N", Transform( xLim, "@E 999,999.99" ), ;
              AllTrim( hb_CStr( xLim ) ) ) } )
      oSrc:Skip( 1 )
   ENDDO
   RETURN aData
