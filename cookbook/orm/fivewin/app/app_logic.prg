/* app_logic.prg -- nucleo de dados reusado pela GUI e pelo smoke /auto. Sem
   FiveWin. Busca textual em memoria (o builder nao tem Like; uniforme em todos
   os backends). */
#include "hborm.ch"

FUNCTION ListarClientes( oConn, cFiltro, lComExcluidos )
   LOCAL oQ := TCliente():New( oConn )
   LOCAL aAll
   IF lComExcluidos == .T.
      oQ:WithTrashed()
   ENDIF
   aAll := oQ:All()
   RETURN FiltrarTexto( aAll, cFiltro, { "nome", "cidade" } )

FUNCTION ListarPedidos( oConn, nClienteId, lComExcluidos )
   LOCAL oQ := TPedido():New( oConn ):Com( "cliente" )
   LOCAL aAll, aOut := {}, o
   IF lComExcluidos == .T.
      oQ:WithTrashed()
   ENDIF
   aAll := oQ:All()
   FOR EACH o IN aAll
      IF nClienteId == NIL .OR. o:Get( "cliente_id" ) == nClienteId
         AAdd( aOut, o )
      ENDIF
   NEXT
   RETURN aOut

FUNCTION FiltrarTexto( aModels, cFiltro, aCols )
   LOCAL aOut := {}, o, cCol, lHit
   IF cFiltro == NIL .OR. Empty( cFiltro )
      RETURN aModels
   ENDIF
   cFiltro := Upper( AllTrim( cFiltro ) )
   FOR EACH o IN aModels
      lHit := .F.
      FOR EACH cCol IN aCols
         IF At( cFiltro, Upper( AllTrim( hb_CStr( o:Get( cCol ) ) ) ) ) > 0
            lHit := .T.
            EXIT
         ENDIF
      NEXT
      IF lHit
         AAdd( aOut, o )
      ENDIF
   NEXT
   RETURN aOut

FUNCTION SalvarCliente( oConn, hAttrs )
   LOCAL oM
   IF hb_HHasKey( hAttrs, "id" ) .AND. ! Empty( hAttrs[ "id" ] )
      oM := TCliente():New( oConn ):Find( hAttrs[ "id" ] )
      IF oM == NIL
         oM := TCliente():New( oConn )
      ENDIF
      HSetAll( oM, hAttrs )
      oM:Save()
   ELSE
      oM := TCliente():New( oConn ):Create( hAttrs )
      FetchLastId( oConn, oM )
   ENDIF
   RETURN oM

FUNCTION SalvarPedido( oConn, hAttrs )
   LOCAL oM
   IF hb_HHasKey( hAttrs, "id" ) .AND. ! Empty( hAttrs[ "id" ] )
      oM := TPedido():New( oConn ):Find( hAttrs[ "id" ] )
      IF oM == NIL
         oM := TPedido():New( oConn )
      ENDIF
      HSetAll( oM, hAttrs )
      oM:Save()
   ELSE
      oM := TPedido():New( oConn ):Create( hAttrs )
      FetchLastId( oConn, oM )
   ENDIF
   RETURN oM

FUNCTION ExcluirRegistro( oModel )
   RETURN oModel:Delete()

FUNCTION RestaurarRegistro( oModel )
   RETURN oModel:Restore()

STATIC PROCEDURE FetchLastId( oConn, oM )
   LOCAL aId
   /* backends SQL sem auto-fetch de PK: recupera o last_insert_rowid (per-conexao no SQLite).
      Nav (DBF/ADT) ja recebe o PK do NavInsert -> nao entra aqui. */
   IF Empty( oM:Get( "id" ) ) .AND. ! oConn:IsNavigational()
      aId := oConn:Query( "SELECT last_insert_rowid() AS rid", {} )
      IF Len( aId ) > 0 .AND. ! Empty( aId[ 1 ][ "rid" ] )
         oM:Set( "id", aId[ 1 ][ "rid" ] )
      ENDIF
   ENDIF
   RETURN

STATIC PROCEDURE HSetAll( oM, hAttrs )
   LOCAL cK
   FOR EACH cK IN hb_HKeys( hAttrs )
      oM:Set( cK, hAttrs[ cK ] )
   NEXT
   RETURN
