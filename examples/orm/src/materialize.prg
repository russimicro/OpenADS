/* materialize.prg -- materializa um result-set (array de row-hashes) numa
   tabela navegacional + devolve um TORMCursor p/ browse. Schema inferido dos
   VALORES (AutoInc vira N na copia). */
#include "hborm.ch"
#include "error.ch"

/* nº de casas decimais de um numero (via string, sem assumir SET DECIMALS) */
STATIC FUNCTION DecimalsOf( xVal )
   LOCAL cS := hb_ntos( xVal ), nDot := At( ".", cS )
   RETURN iif( nDot == 0, 0, Len( cS ) - nDot )

/* infere o AST de colunas a partir dos valores de aRows (chaves de aRows[1]) */
FUNCTION ORM_InferColumns( aRows )
   LOCAL aCols := {}, cKey, hRow, xVal
   LOCAL lSeen, lLog, lDate, lNum, lStr, lAllInt, nMaxScale, nMaxLen
   IF Empty( aRows )
      RETURN {}
   ENDIF
   FOR EACH cKey IN hb_HKeys( aRows[ 1 ] )
      lSeen := lLog := lDate := lNum := lStr := .F.
      lAllInt := .T. ; nMaxScale := 0 ; nMaxLen := 1
      FOR EACH hRow IN aRows
         xVal := hb_HGetDef( hRow, cKey, NIL )
         IF xVal == NIL ; LOOP ; ENDIF
         lSeen := .T.
         DO CASE
         CASE HB_ISLOGICAL( xVal ) ; lLog := .T.
         CASE HB_ISDATE( xVal )    ; lDate := .T.
         CASE HB_ISNUMERIC( xVal )
            lNum := .T.
            IF xVal != Int( xVal )
               lAllInt := .F.
               nMaxScale := Max( nMaxScale, DecimalsOf( xVal ) )
            ENDIF
         OTHERWISE
            lStr := .T.
            nMaxLen := Max( nMaxLen, Len( hb_CStr( xVal ) ) )
         ENDCASE
      NEXT
      DO CASE
      CASE ! lSeen
         AAdd( aCols, { "name" => cKey, "type" => "string", "len" => 10 } )
      CASE lLog .AND. ! lNum .AND. ! lDate .AND. ! lStr
         AAdd( aCols, { "name" => cKey, "type" => "boolean" } )
      CASE lDate .AND. ! lNum .AND. ! lStr
         AAdd( aCols, { "name" => cKey, "type" => "date" } )
      CASE lNum .AND. ! lStr
         IF lAllInt
            AAdd( aCols, { "name" => cKey, "type" => "integer" } )
         ELSE
            AAdd( aCols, { "name" => cKey, "type" => "decimal", ;
                           "prec" => 18, "scale" => Min( nMaxScale, 8 ) } )
         ENDIF
      OTHERWISE
         AAdd( aCols, { "name" => cKey, "type" => "string", "len" => Max( nMaxLen, 1 ) } )
      ENDCASE
   NEXT
   RETURN aCols

/* normaliza hOpts["indexes"] (lista de listas-de-colunas) p/ o shape que o
   NavCreateTable consome: { "columns"=>{...} } por indice */
STATIC FUNCTION MatIndexes( aIdxIn )
   LOCAL aOut := {}, a
   IF aIdxIn == NIL ; RETURN {} ; ENDIF
   FOR EACH a IN aIdxIn
      AAdd( aOut, { "columns" => a } )
   NEXT
   RETURN aOut

STATIC PROCEDURE MaterializeRaise( cWhy )
   LOCAL oErr := ErrorNew()
   oErr:Subsystem := "hb_orm"
   oErr:SubCode := 1300
   oErr:Severity := ES_ERROR
   oErr:Description := "materializacao falhou: " + cWhy   // sem SQL/path
   oErr:Operation := "ORM_Materialize"
   oErr:CanRetry := .F.
   Eval( ErrorBlock(), oErr )
   RETURN

/* materializa aRows numa tabela nav + devolve TORMCursor aberto */
FUNCTION ORM_Materialize( oNavConn, cTable, aRows, hOpts )
   LOCAL aCols, aIdx, hRow
   IF hOpts == NIL ; hOpts := hb_Hash() ; ENDIF
   IF oNavConn == NIL .OR. ! oNavConn:IsNavigational()
      MaterializeRaise( "destino precisa ser navegacional" )
   ENDIF
   IF Empty( aRows )
      MaterializeRaise( "result-set vazio: schema indeterminado" )
   ENDIF
   aCols := ORM_InferColumns( aRows )
   aIdx  := MatIndexes( hb_HGetDef( hOpts, "indexes", {} ) )
   /* drop-se-existir (inocuo se nao existe); identificador quotado p/ casar
      com o create/insert (QuoteIdent), evitando drop-miss em nome reservado */
   BEGIN SEQUENCE WITH {| e | Break( e ) }
      oNavConn:Execute( "DROP TABLE " + TORMGrammar():New():QuoteIdent( cTable ) )
   END SEQUENCE
   NavCreateTable( oNavConn, { "table" => cTable, "columns" => aCols, "indexes" => aIdx } )
   FOR EACH hRow IN aRows
      NavInsertRaw( oNavConn, cTable, hRow )
   NEXT
   RETURN TORMCursor():New( oNavConn, cTable ):Open()
