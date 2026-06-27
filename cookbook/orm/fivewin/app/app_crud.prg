/* app_crud.prg -- dialogos CRUD (Clientes e Pedidos).
 * Sem dominio gaia/llmpss. Sem FiveWin no repo. */

#include "FiveWin.ch"
#include "hborm.ch"

/* ---------------------------------------------------------------- */
/* FUNCTION EditarCliente                                            */
/*   oModel == NIL -> novo; != NIL -> editar existente              */
/*   Retorna .T. se gravou, .F. se cancelou                         */
/* ---------------------------------------------------------------- */

FUNCTION EditarCliente( oConn, oModel, oOwner )
   LOCAL oDlg
   LOCAL cNome, cCidade, nLimite
   LOCAL lGravou := .F.
   LOCAL hAttrs

   /* pre-popula campos a partir do model ou defaults */
   IF oModel != NIL
      cNome   := AllTrim( hb_CStr( oModel:Get( "nome" ) ) )
      cCidade := AllTrim( hb_CStr( oModel:Get( "cidade" ) ) )
      nLimite := oModel:Get( "limite" )
   ELSE
      cNome   := ""
      cCidade := ""
      nLimite := 0.00
   ENDIF

   IF ValType( nLimite ) != "N"
      nLimite := 0.00
   ENDIF

   cNome   := PadR( cNome,   40 )
   cCidade := PadR( cCidade, 30 )

   DEFINE DIALOG oDlg ;
      TITLE iif( oModel == NIL, "Novo Cliente", "Editar Cliente" ) ;
      SIZE 400, 200

   @ 0.5,  2 SAY "Nome:"   OF oDlg
   @ 1.6,  2 SAY "Cidade:" OF oDlg
   @ 2.7,  2 SAY "Limite:" OF oDlg

   @ 0.5, 10 GET cNome   OF oDlg SIZE 220, 12
   @ 1.6, 10 GET cCidade OF oDlg SIZE 160, 12
   @ 2.7, 10 GET nLimite OF oDlg PICTURE "@E 999,999.99" SIZE 80, 12

   @ 4.5, 10 BUTTON "&Gravar" OF oDlg SIZE 60, 14 ACTION ;
      iif( Empty( AllTrim( cNome ) ), ;
           MsgAlert( "Nome e obrigatorio" ), ;
           iif( ValType( nLimite ) != "N" .OR. nLimite < 0, ;
                MsgAlert( "Limite deve ser 0 ou positivo" ), ;
                ( hAttrs := { "nome"    => AllTrim( cNome ), ;
                              "cidade"  => AllTrim( cCidade ), ;
                              "limite"  => nLimite }, ;
                  iif( oModel != NIL .AND. ! Empty( oModel:Get( "id" ) ), ;
                       hAttrs[ "id" ] := oModel:Get( "id" ), NIL ), ;
                  SalvarCliente( oConn, hAttrs ), ;
                  lGravou := .T., ;
                  oDlg:End( IDOK ) ) ) )

   @ 4.5, 22 BUTTON "&Cancelar" OF oDlg SIZE 60, 14 ACTION oDlg:End()

   ACTIVATE DIALOG oDlg CENTERED

   RETURN lGravou

/* ---------------------------------------------------------------- */
/* FUNCTION EditarPedido                                             */
/* ---------------------------------------------------------------- */

FUNCTION EditarPedido( oConn, oModel, oOwner )
   LOCAL oDlg
   LOCAL aClientes, aCliNomes, nCliIdx
   LOCAL cDesc, nValor, dData
   LOCAL lGravou := .F.
   LOCAL hAttrs
   LOCAL o, i

   /* monta lista de clientes para o combobox */
   aClientes := ListarClientes( oConn, , .F. )
   aCliNomes := {}
   FOR EACH o IN aClientes
      AAdd( aCliNomes, AllTrim( hb_CStr( o:Get( "nome" ) ) ) )
   NEXT

   IF Len( aCliNomes ) == 0
      MsgAlert( "Nenhum cliente cadastrado -- cadastre um cliente antes" )
      RETURN .F.
   ENDIF

   /* pre-popula */
   nCliIdx := 1
   IF oModel != NIL
      cDesc  := AllTrim( hb_CStr( oModel:Get( "descricao" ) ) )
      nValor := oModel:Get( "valor" )
      dData  := oModel:Get( "data" )
      IF ValType( nValor ) != "N" ; nValor := 0.00 ; ENDIF
      IF ValType( dData  ) != "D" ; dData  := Date() ; ENDIF
      /* localiza o cliente no combo */
      i := 1
      FOR EACH o IN aClientes
         IF hb_CStr( o:Get( "id" ) ) == hb_CStr( oModel:Get( "cliente_id" ) )
            nCliIdx := i
            EXIT
         ENDIF
         i++
      NEXT
   ELSE
      cDesc  := ""
      nValor := 0.00
      dData  := Date()
   ENDIF

   cDesc := PadR( cDesc, 60 )

   DEFINE DIALOG oDlg ;
      TITLE iif( oModel == NIL, "Novo Pedido", "Editar Pedido" ) ;
      SIZE 420, 230

   /* combobox ANTES dos outros (menor z-order) */
   @ 0.5, 11 COMBOBOX oCombo VAR nCliIdx ITEMS aCliNomes OF oDlg SIZE 200, 80

   @ 0.5,  2 SAY "Cliente:"   OF oDlg
   @ 1.6,  2 SAY "Descricao:" OF oDlg
   @ 2.7,  2 SAY "Valor:"     OF oDlg
   @ 3.8,  2 SAY "Data:"      OF oDlg

   @ 1.6, 11 GET cDesc  OF oDlg SIZE 220, 12
   @ 2.7, 11 GET nValor OF oDlg PICTURE "@E 999,999.99" SIZE 80, 12
   @ 3.8, 11 GET dData  OF oDlg SIZE 80,  12

   @ 5.5, 10 BUTTON "&Gravar" OF oDlg SIZE 60, 14 ACTION ;
      iif( nCliIdx < 1 .OR. nCliIdx > Len( aClientes ), ;
           MsgAlert( "Selecione um cliente" ), ;
           iif( ValType( nValor ) != "N" .OR. nValor < 0, ;
                MsgAlert( "Valor deve ser 0 ou positivo" ), ;
                ( hAttrs := { "cliente_id" => aClientes[ nCliIdx ]:Get( "id" ), ;
                              "descricao"  => AllTrim( cDesc ), ;
                              "valor"      => nValor, ;
                              "data"       => dData }, ;
                  iif( oModel != NIL .AND. ! Empty( oModel:Get( "id" ) ), ;
                       hAttrs[ "id" ] := oModel:Get( "id" ), NIL ), ;
                  SalvarPedido( oConn, hAttrs ), ;
                  lGravou := .T., ;
                  oDlg:End( IDOK ) ) ) )

   @ 5.5, 22 BUTTON "&Cancelar" OF oDlg SIZE 60, 14 ACTION oDlg:End()

   ACTIVATE DIALOG oDlg CENTERED

   RETURN lGravou
