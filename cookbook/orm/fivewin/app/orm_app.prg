/* orm_app.prg -- app MDI principal: menu de backend + cadastros.
 * Modo /selftest: headless, sem janela, exercita helpers de dados da GUI.
 *
 * Uso:
 *   orm_app.exe               -- abre a janela MDI
 *   orm_app.exe /selftest     -- modo headless (CI): grava app_selftest_result.txt
 *
 * FiveWin e COMERCIAL. Nenhum fonte/lib FWH entra no repo; o build.bat
 * apenas referencia a instalacao local via FWDIR64. */

#include "FiveWin.ch"
#include "hborm.ch"

/* Manter simbolos ADS + hbo no binario */
REQUEST ADSCDX, ADSNTX
REQUEST HBO_SETTABLETYPE

/* ------- estado global do app ------- */
STATIC s_oConn := NIL
STATIC s_oWnd
STATIC s_oMsg

/* ===================================================================== */
/*  FUNCTION Main                                                         */
/* ===================================================================== */

FUNCTION Main( cArg )

   /* --- modo headless /selftest (sem janela) --- */
   IF ! Empty( cArg ) .AND. Lower( AllTrim( cArg ) ) == "/selftest"
      RETURN SelfTest()
   ENDIF

   /* --- modo GUI: janela MDI --- */
   DEFINE WINDOW s_oWnd FROM 1, 1 TO 30, 120 ;
      TITLE "hb_orm2 -- App MDI (FiveWin)" ;
      MDI MENU CriaMenu()

   DEFINE MSGBAR s_oMsg OF s_oWnd ;
      PROMPT "Nenhum backend ativo -- escolha em Backend"

   ACTIVATE WINDOW s_oWnd MAXIMIZED ;
      VALID MsgYesNo( "Sair do hb_orm2?" )

   IF s_oConn != NIL
      s_oConn:Close()
      s_oConn := NIL
   ENDIF

   RETURN NIL

/* ===================================================================== */
/*  CriaMenu -- monta barra de menus do MDI frame                        */
/* ===================================================================== */

STATIC FUNCTION CriaMenu()
   LOCAL oMenu

   MENU oMenu
      MENUITEM "&Backend"
      MENU
         MENUITEM "SQLite"  ACTION TrocaBackend( "sqlite" )
         MENUITEM "DBF"     ACTION TrocaBackend( "dbf" )
         MENUITEM "ADT"     ACTION TrocaBackend( "adt" )
         MENUITEM "MariaDB" ACTION TrocaBackend( "maria" )
      ENDMENU

      MENUITEM "&Cadastros"
      MENU
         MENUITEM "Clientes" ACTION ;
            iif( s_oConn != NIL, ;
                 BrowseClientes( s_oConn, s_oWnd ), ;
                 MsgAlert( "Escolha um backend em Backend primeiro" ) )
         MENUITEM "Pedidos" ACTION ;
            iif( s_oConn != NIL, ;
                 BrowsePedidos( s_oConn, s_oWnd, NIL ), ;
                 MsgAlert( "Escolha um backend em Backend primeiro" ) )
      ENDMENU

      MENUITEM "&Ajuda"
      MENU
         MENUITEM "Sobre" ACTION ;
            MsgInfo( "hb_orm2 v0.1" + hb_eol() + ;
                     "ORM para Harbour / FiveWin (exemplo didatico)" + hb_eol() + ;
                     "FiveWin e COMERCIAL (recompilar exige licenca)" + hb_eol() + ;
                     "github.com/Admnwk/hb_orm2", ;
                     "Sobre" )
      ENDMENU
   ENDMENU

   RETURN oMenu

/* ===================================================================== */
/*  TrocaBackend -- fecha conn atual e abre novo backend                 */
/* ===================================================================== */

STATIC PROCEDURE TrocaBackend( cTipo )
   LOCAL hR

   hbo_SetTableType( 0 )              // reset defensivo

   IF s_oConn != NIL
      s_oConn:Close()
      s_oConn := NIL
   ENDIF

   hR := AbrirBackend( cTipo )

   IF ! hR[ "ok" ]
      s_oMsg:SetText( "Backend " + Upper( cTipo ) + ": INDISPONIVEL -- " + hR[ "motivo" ] )
      MsgInfo( "Backend indisponivel:" + hb_eol() + hR[ "motivo" ], Upper( cTipo ) )
   ELSE
      s_oConn := hR[ "conn" ]
      SeedDados( s_oConn )
      s_oMsg:SetText( "Backend ativo: " + Upper( cTipo ) + " -- ok (mostrando Clientes)" )
      /* feedback visual imediato: abre o browse de Clientes ao trocar de backend */
      BrowseClientes( s_oConn, s_oWnd )
   ENDIF

   RETURN

/* ===================================================================== */
/*  SelfTest -- headless: exercita LinhasCliente/LinhasPedido sem janela */
/* ===================================================================== */

STATIC FUNCTION SelfTest()
   LOCAL hR := AbrirBackend( "sqlite" ), oConn, aCli, aPed, aLin
   LOCAL nFail := 0, cLog := ""

   IF ! hR[ "ok" ]
      ? "selftest: nao abriu sqlite"
      ErrorLevel( 1 )
      RETURN 1
   ENDIF

   oConn := hR[ "conn" ]
   SeedDados( oConn )

   aCli := ListarClientes( oConn, , .F. )
   aLin := LinhasCliente( aCli )
   nFail += iif( Len( aLin ) == 6, 0, 1 )                               // 6 clientes
   nFail += iif( Len( aLin ) > 0 .AND. Len( aLin[ 1 ] ) == 5, 0, 1 )   // 5 colunas

   aPed := ListarPedidos( oConn, , .F. )
   aLin := LinhasPedido( aPed )
   nFail += iif( Len( aLin ) == 6, 0, 1 )                               // 6 pedidos
   nFail += iif( Len( aLin ) > 0 .AND. aLin[ 1 ][ 2 ] != "?", 0, 1 )   // cliente eager ok

   cLog := "selftest GUI: clientes=" + hb_CStr( Len( aCli ) ) + ;
           " pedidos=" + hb_CStr( Len( aPed ) ) + ;
           " falhas=" + hb_CStr( nFail ) + hb_eol()

   hb_MemoWrit( hb_DirBase() + "app_selftest_result.txt", cLog )
   ? cLog

   oConn:Close()

   ErrorLevel( iif( nFail == 0, 0, 1 ) )
   RETURN iif( nFail == 0, 0, 1 )
