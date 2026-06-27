/* app_browse.prg -- janelas MDI-filhas de browse (Clientes e Pedidos).
 * Usa xBrowse em modo ARRAY (caminho de render confirmado).
 * Dominio generico (sem gaia/llmpss). FiveWin COMERCIAL: nenhum fonte FWH no repo.
 * REGRA: DEFINE BUTTON ... ACTION DEVE estar em 1 linha (sem ';' de continuacao). */

#include "FiveWin.ch"
#include "xbrowse.ch"
#include "hborm.ch"

REQUEST ADSCDX, ADSNTX
REQUEST ADSKEYNO, ADSKEYCOUNT, ADSGETRELKEYPOS, ADSSETRELKEYPOS

/* ---------------------------------------------------------------- */
/* Helpers de linha (1 linha = 1 chamada; bGetModels = {|| aModels}) */
/* ---------------------------------------------------------------- */

STATIC PROCEDURE AcaoCliNovo( oConn, oChild, bRel )
   EditarCliente( oConn, NIL, oChild )
   Eval( bRel )
   RETURN

STATIC PROCEDURE AcaoCliEdit( oConn, oChild, oBrw, bGM, bRel )
   LOCAL n := oBrw:nArrayAt, aM := Eval( bGM )
   IF n >= 1 .AND. n <= Len( aM )
      EditarCliente( oConn, aM[ n ], oChild )
      Eval( bRel )
   ELSE
      MsgAlert( "Selecione um cliente" )
   ENDIF
   RETURN

STATIC PROCEDURE AcaoCliDel( oBrw, bGM, bRel )
   LOCAL n := oBrw:nArrayAt, aM := Eval( bGM )
   IF n >= 1 .AND. n <= Len( aM )
      IF MsgYesNo( "Excluir este cliente?" )
         ExcluirRegistro( aM[ n ] )
         Eval( bRel )
      ENDIF
   ELSE
      MsgAlert( "Selecione um cliente" )
   ENDIF
   RETURN

STATIC PROCEDURE AcaoCliPed( oConn, oMDI, oBrw, bGM )
   LOCAL n := oBrw:nArrayAt, aM := Eval( bGM )
   IF n >= 1 .AND. n <= Len( aM )
      BrowsePedidos( oConn, oMDI, aM[ n ]:Get( "id" ) )
   ELSE
      MsgAlert( "Selecione um cliente" )
   ENDIF
   RETURN

STATIC PROCEDURE AcaoPedNovo( oConn, oChild, bRel )
   EditarPedido( oConn, NIL, oChild )
   Eval( bRel )
   RETURN

STATIC PROCEDURE AcaoPedEdit( oConn, oChild, oBrw, bGM, bRel )
   LOCAL n := oBrw:nArrayAt, aM := Eval( bGM )
   IF n >= 1 .AND. n <= Len( aM )
      EditarPedido( oConn, aM[ n ], oChild )
      Eval( bRel )
   ELSE
      MsgAlert( "Selecione um pedido" )
   ENDIF
   RETURN

STATIC PROCEDURE AcaoPedDel( oBrw, bGM, bRel )
   LOCAL n := oBrw:nArrayAt, aM := Eval( bGM )
   IF n >= 1 .AND. n <= Len( aM )
      IF MsgYesNo( "Excluir este pedido?" )
         ExcluirRegistro( aM[ n ] )
         Eval( bRel )
      ENDIF
   ELSE
      MsgAlert( "Selecione um pedido" )
   ENDIF
   RETURN

/* ---------------------------------------------------------------- */
/* PUBLICA: materializa clientes -> array de arrays de string        */
/* ---------------------------------------------------------------- */

FUNCTION LinhasCliente( aModels )
   LOCAL a := {}, o, xLim
   FOR EACH o IN aModels
      xLim := o:Get( "limite" )
      AAdd( a, { ;
         hb_CStr( o:Get( "id" ) ), ;
         AllTrim( hb_CStr( o:Get( "nome" ) ) ), ;
         AllTrim( hb_CStr( o:Get( "cidade" ) ) ), ;
         iif( ValType( xLim ) == "N", Transform( xLim, "@E 999,999.99" ), "" ), ;
         iif( o:Trashed(), "EXCLUIDO", "ativo" ) } )
   NEXT
   RETURN a

/* ---------------------------------------------------------------- */
/* PUBLICA: materializa pedidos (cliente via eager belongsTo)        */
/* ---------------------------------------------------------------- */

FUNCTION LinhasPedido( aModels )
   LOCAL a := {}, o, oCli, xVal
   FOR EACH o IN aModels
      oCli := o:Rel( "cliente" )
      xVal := o:Get( "valor" )
      AAdd( a, { ;
         hb_CStr( o:Get( "id" ) ), ;
         iif( oCli == NIL, "?", AllTrim( hb_CStr( oCli:Get( "nome" ) ) ) ), ;
         AllTrim( hb_CStr( o:Get( "descricao" ) ) ), ;
         iif( ValType( xVal ) == "N", Transform( xVal, "@E 999,999.99" ), "" ), ;
         DToC( o:Get( "data" ) ) } )
   NEXT
   RETURN a

/* ---------------------------------------------------------------- */
/* FUNCTION BrowseClientes -- MDICHILD com xBrowse ARRAY             */
/* ---------------------------------------------------------------- */

FUNCTION BrowseClientes( oConn, oMDI )
   LOCAL oChild, oBrw, oBar
   LOCAL aModels, aData
   LOCAL cBusca := Space( 30 )
   LOCAL lExcl  := .F.
   LOCAL bRel, bGM
   LOCAL aHead := { "Id", "Nome", "Cidade", "Limite", "Status" }
   LOCAL i

   aModels := ListarClientes( oConn, , .F. )
   aData   := LinhasCliente( aModels )

   bRel := {|| aModels := ListarClientes( oConn, AllTrim( cBusca ), lExcl ), aData := LinhasCliente( aModels ), oBrw:SetArray( aData ), oBrw:Refresh() }
   bGM  := {|| aModels }

   DEFINE WINDOW oChild MDICHILD OF oMDI FROM 2, 5 TO 24, 115 TITLE "Clientes"

   DEFINE BUTTONBAR oBar OF oChild

   DEFINE BUTTON OF oBar PROMPT "Novo"    ACTION AcaoCliNovo( oConn, oChild, bRel )
   DEFINE BUTTON OF oBar PROMPT "Editar"  ACTION AcaoCliEdit( oConn, oChild, oBrw, bGM, bRel )
   DEFINE BUTTON OF oBar PROMPT "Excluir" ACTION AcaoCliDel( oBrw, bGM, bRel )
   DEFINE BUTTON OF oBar PROMPT "Pedidos" ACTION AcaoCliPed( oConn, oMDI, oBrw, bGM )

   @ 3, 265 GET cBusca OF oBar SIZE 155, 22 PIXEL ON CHANGE Eval( bRel )
   @ 3, 428 CHECKBOX lExcl PROMPT "Excluidos" OF oBar SIZE 100, 20 PIXEL ON CHANGE Eval( bRel )

   @ 0, 0 XBROWSE oBrw OF oChild ARRAY aData AUTOCOLS CELL LINES NOBORDER

   FOR i := 1 TO Len( aHead )
      IF i <= Len( oBrw:aCols )
         oBrw:aCols[ i ]:cHeader := aHead[ i ]
      ENDIF
   NEXT

   oBrw:CreateFromCode()
   oChild:oClient := oBrw

   ACTIVATE WINDOW oChild ON INIT ( oBrw:SetFocus(), oBrw:GoTop(), oBrw:Refresh() ) ON RESIZE ( oBrw:adjust(), oBrw:Refresh() )

   RETURN NIL

/* ---------------------------------------------------------------- */
/* FUNCTION BrowsePedidos -- MDICHILD com xBrowse ARRAY de pedidos   */
/* ---------------------------------------------------------------- */

FUNCTION BrowsePedidos( oConn, oMDI, nClienteId )
   LOCAL oChild, oBrw, oBar
   LOCAL aModels, aData
   LOCAL cBusca := Space( 30 )
   LOCAL lExcl  := .F.
   LOCAL bRel, bGM
   LOCAL cTitle := "Pedidos" + iif( nClienteId != NIL, " - cli " + hb_CStr( nClienteId ), "" )
   LOCAL aHead := { "Id", "Cliente", "Descricao", "Valor", "Data" }
   LOCAL i

   aModels := ListarPedidos( oConn, nClienteId, .F. )
   aData   := LinhasPedido( aModels )

   bRel := {|| aModels := ListarPedidos( oConn, nClienteId, lExcl ), aData := LinhasPedido( aModels ), oBrw:SetArray( aData ), oBrw:Refresh() }
   bGM  := {|| aModels }

   DEFINE WINDOW oChild MDICHILD OF oMDI FROM 3, 5 TO 24, 115 TITLE cTitle

   DEFINE BUTTONBAR oBar OF oChild

   DEFINE BUTTON OF oBar PROMPT "Novo"    ACTION AcaoPedNovo( oConn, oChild, bRel )
   DEFINE BUTTON OF oBar PROMPT "Editar"  ACTION AcaoPedEdit( oConn, oChild, oBrw, bGM, bRel )
   DEFINE BUTTON OF oBar PROMPT "Excluir" ACTION AcaoPedDel( oBrw, bGM, bRel )

   @ 3, 200 GET cBusca OF oBar SIZE 155, 22 PIXEL ON CHANGE Eval( bRel )
   @ 3, 363 CHECKBOX lExcl PROMPT "Excluidos" OF oBar SIZE 100, 20 PIXEL ON CHANGE Eval( bRel )

   @ 0, 0 XBROWSE oBrw OF oChild ARRAY aData AUTOCOLS CELL LINES NOBORDER

   FOR i := 1 TO Len( aHead )
      IF i <= Len( oBrw:aCols )
         oBrw:aCols[ i ]:cHeader := aHead[ i ]
      ENDIF
   NEXT

   oBrw:CreateFromCode()
   oChild:oClient := oBrw

   ACTIVATE WINDOW oChild ON INIT ( oBrw:SetFocus(), oBrw:GoTop(), oBrw:Refresh() ) ON RESIZE ( oBrw:adjust(), oBrw:Refresh() )

   RETURN NIL
