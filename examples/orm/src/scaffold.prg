/* scaffold.prg -- Porta A (avancado): gera a string de um .prg a partir do
   catalogo introspectado. Dois geradores:
     - ORM_Scaffold        -> CREATE CLASS ... FROM TORMModel (Casts + ReadOnly)
     - ORM_ScaffoldSchema  -> FUNCTION Schema_X( oConn ) c/ TORMBlueprint
                              (Nullable / Default / Primary / Id) */
#include "hborm.ch"

/* ---- scaffold de MODEL ---------------------------------------------------- */
FUNCTION ORM_Scaffold( cTable, oConn, hOpts )
   LOCAL aCols, cSrc
   IF oConn == NIL
      oConn := TORMConnection_Default()
   ENDIF
   aCols := ORM_Introspect( oConn, cTable )
   cSrc  := ORM_ScaffoldFromCols( cTable, aCols )
   IF hOpts != NIL .AND. hb_HHasKey( hOpts, "path" )
      hb_MemoWrit( hOpts[ "path" ], cSrc )
   ENDIF
   RETURN cSrc

/* nucleo cols-driven do scaffold de model (testavel sem conexao) */
FUNCTION ORM_ScaffoldFromCols( cTable, aCols )
   LOCAL cClass, cSrc, cNl := hb_eol(), aRO
   cClass := ScaffoldClassName( cTable )
   cSrc := "/* Gerado por ORM_Scaffold a partir da tabela '" + cTable + "'." + cNl
   cSrc += "   Edite a vontade: e um ponto de partida, nao um arquivo gerenciado. */" + cNl
   cSrc += DictHintsBlock( aCols )            // "" quando nenhuma coluna tem comment/rule
   cSrc += '#include "hborm.ch"' + cNl + cNl
   cSrc += "CREATE CLASS " + cClass + " FROM TORMModel" + cNl
   cSrc += '   METHOD TableName()  INLINE "' + cTable + '"' + cNl
   cSrc += '   METHOD PrimaryKey() INLINE "' + ORM_PkFromCols( aCols ) + '"' + cNl
   cSrc += "   METHOD Casts()      INLINE " + CastsLiteral( aCols ) + cNl
   aRO := ORM_ReadOnlyFromCols( aCols )
   IF Len( aRO ) > 0
      cSrc += "   METHOD ReadOnly()      INLINE " + StrLiteralArray( aRO ) + cNl
   ENDIF
   cSrc += "END CLASS" + cNl
   RETURN cSrc

/* ---- scaffold de SCHEMA (Blueprint) -------------------------------------- */
FUNCTION ORM_ScaffoldSchema( cTable, oConn, hOpts )
   LOCAL aCols, cSrc
   IF oConn == NIL
      oConn := TORMConnection_Default()
   ENDIF
   aCols := ORM_Introspect( oConn, cTable )
   cSrc  := ORM_ScaffoldSchemaFromCols( cTable, aCols )
   IF hOpts != NIL .AND. hb_HHasKey( hOpts, "path" )
      hb_MemoWrit( hOpts[ "path" ], cSrc )
   ENDIF
   RETURN cSrc

/* nucleo cols-driven do scaffold de schema (testavel sem conexao) */
FUNCTION ORM_ScaffoldSchemaFromCols( cTable, aCols )
   LOCAL cClass, cSrc, cNl := hb_eol(), aLines := {}, i
   cClass := ScaffoldClassName( cTable )
   cSrc := "/* Gerado por ORM_ScaffoldSchema a partir da tabela '" + cTable + "'." + cNl
   cSrc += "   Edite a vontade: e um ponto de partida, nao um arquivo gerenciado. */" + cNl
   cSrc += '#include "hborm.ch"' + cNl + cNl
   cSrc += "FUNCTION Schema_" + cClass + "( oConn )" + cNl
   cSrc += '   RETURN TORMSchema():New( oConn ):CreateTable( "' + cTable + '", {| t | ;' + cNl
   FOR i := 1 TO Len( aCols )
      AAdd( aLines, "      t" + ColBlueprintCall( aCols[ i ] ) )
   NEXT
   FOR i := 1 TO Len( aLines )
      cSrc += aLines[ i ] + iif( i < Len( aLines ), ", ;", " } )" ) + cNl
   NEXT
   RETURN cSrc

/* uma coluna do catalogo -> a cadeia de chamadas do Blueprint.
   autoinc (pk gerido pela engine) -> :Id() cobre tipo+pk+not null. */
STATIC FUNCTION ColBlueprintCall( hCol )
   LOCAL cName := hCol[ "nome" ], cCast := hb_HGetDef( hCol, "cast", "string" )
   LOCAL lPk   := hb_HGetDef( hCol, "pk", .F. )
   LOCAL lRO   := hb_HGetDef( hCol, "readonly", .F. )
   LOCAL lNull := hb_HGetDef( hCol, "nullable", .T. )
   LOCAL xDef  := hb_HGetDef( hCol, "default", NIL )
   LOCAL c
   IF lPk .AND. lRO
      RETURN ':Id( "' + cName + '" )'
   ENDIF
   c := TypeCall( cCast, cName )
   IF lPk
      c += ":Primary()"
   ELSEIF ! lNull
      c += ":Nullable( .F. )"
   ENDIF
   IF xDef != NIL
      c += ":Default( " + DefaultLiteral( xDef ) + " )"
   ENDIF
   RETURN c

/* token de cast -> metodo de tipo do Blueprint */
STATIC FUNCTION TypeCall( cCast, cName )
   LOCAL nScale
   DO CASE
   CASE cCast == "integer"  ; RETURN ':Integer( "' + cName + '" )'
   CASE cCast == "boolean"  ; RETURN ':Boolean( "' + cName + '" )'
   CASE cCast == "date"     ; RETURN ':Date( "' + cName + '" )'
   CASE cCast == "datetime" ; RETURN ':DateTime( "' + cName + '" )'
   CASE Left( cCast, 7 ) == "decimal"
      nScale := DecScale( cCast )
      RETURN iif( nScale > 0, ;
         ':Decimal( "' + cName + '", NIL, ' + LTrim( Str( nScale ) ) + ' )', ;
         ':Decimal( "' + cName + '" )' )
   ENDCASE
   RETURN ':String( "' + cName + '" )'

/* "decimal:2" -> 2 ; "decimal" -> 0 */
STATIC FUNCTION DecScale( cCast )
   LOCAL n := At( ":", cCast )
   RETURN iif( n == 0, 0, Int( Val( SubStr( cCast, n + 1 ) ) ) )

/* default introspectado -> literal Harbour. String entre aspas; resto via hb_CStr. */
STATIC FUNCTION DefaultLiteral( xDef )
   IF HB_ISSTRING( xDef )
      RETURN '"' + xDef + '"'
   ENDIF
   RETURN hb_CStr( xDef )

/* ---- helpers comuns ------------------------------------------------------- */

/* { "id" => "integer", "saldo" => "decimal:2", ... } */
STATIC FUNCTION CastsLiteral( aCols )
   LOCAL c, a := {}
   FOR EACH c IN aCols
      AAdd( a, '"' + c[ "nome" ] + '" => "' + c[ "cast" ] + '"' )
   NEXT
   RETURN "{ " + JoinComma( a ) + " }"

/* { "id", "mtime" } a partir de um array de nomes */
STATIC FUNCTION StrLiteralArray( a )
   LOCAL out := {}, c
   FOR EACH c IN a
      AAdd( out, '"' + c + '"' )
   NEXT
   RETURN "{ " + JoinComma( out ) + " }"

STATIC FUNCTION JoinComma( a )
   LOCAL c := "", i
   FOR i := 1 TO Len( a )
      c += iif( i == 1, "", ", " ) + a[ i ]
   NEXT
   RETURN c

/* bloco de comentario READ-ONLY com as dicas do dicionario (comment/rule).
   Vazio quando nenhuma coluna tem comment nem rule. Nao gera codigo de
   validacao -- so documenta o que o catalogo declara. */
STATIC FUNCTION DictHintsBlock( aCols )
   LOCAL c, aLines := {}, cNl := hb_eol(), cR, cC, cLine
   FOR EACH c IN aCols
      cC := hb_HGetDef( c, "comment", NIL )
      cR := hb_HGetDef( c, "rule", NIL )
      IF cC != NIL .OR. cR != NIL
         cLine := "     " + c[ "nome" ]
         /* neutraliza o fecha-comentario no valor p/ nao encerrar o bloco gerado */
         IF cC != NIL ; cLine += "  -- comment: " + StrTran( cC, "*/", "* /" ) ; ENDIF
         IF cR != NIL ; cLine += iif( cC != NIL, " |", "  --" ) + " rule: " + StrTran( cR, "*/", "* /" ) ; ENDIF
         AAdd( aLines, cLine )
      ENDIF
   NEXT
   IF Len( aLines ) == 0
      RETURN ""
   ENDIF
   RETURN "/* Dicionario (somente leitura -- dicas do catalogo):" + cNl + ;
          ArrToLines( aLines, cNl ) + cNl + "*/" + cNl

/* une array de strings com separador cNl */
STATIC FUNCTION ArrToLines( a, cNl )
   LOCAL c := "", i
   FOR i := 1 TO Len( a )
      c += iif( i == 1, "", cNl ) + a[ i ]
   NEXT
   RETURN c

/* nome de classe sanitizado: "T" + tabela validada e capitalizada */
STATIC FUNCTION ScaffoldClassName( cTable )
   LOCAL cName
   TORMGrammar():New():QuoteIdent( cTable )   // valida; aborta se identificador invalido
   cName := Lower( cTable )
   RETURN "T" + Upper( Left( cName, 1 ) ) + SubStr( cName, 2 )
