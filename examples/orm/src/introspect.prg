/* introspect.prg -- le o catalogo de uma tabela e deriva tokens de cast.
   Despacha por capacidade da conexao: SQL (sqlite/PRAGMA) ou navegacional. */
#include "hborm.ch"
#include "error.ch"

/* propriedades de campo do Data Dictionary (ABI ACE) */
#define DD_FIELD_REQUIRED          305
#define DD_FIELD_DEFAULT           306
#define DD_FIELD_VALIDATION_RULE   307
#define DD_FIELD_VALIDATION_MSG    308
#define DD_FIELD_COMMENT           309

FUNCTION ORM_Introspect( oConn, cTable )
   IF oConn == NIL
      oConn := TORMConnection_Default()
   ENDIF
   IF oConn == NIL .OR. ! oConn:IsOpen()
      IntrospectRaise( "conexao indisponivel" )
   ENDIF
   IF oConn:IsNavigational()
      RETURN IntrospectNav( oConn, cTable )       // estrategia real na Task 4
   ENDIF
   RETURN IntrospectSql( oConn, cTable )

/* ---- estrategia navegacional: dicionario de campos via ACE --------------- */
STATIC FUNCTION IntrospectNav( oConn, cTable )
   LOCAL nTbl, aCols := {}, n, i, cName, hMeta, lHasId := .F., c
   LOCAL lNull, xDef
   nTbl := hbo_OpenTable( oConn:Handle(), cTable )
   IF nTbl == 0
      IntrospectRaise( "tabela nao abre (navegacional)" )
      RETURN {}
   ENDIF
   n := hbo_NumFields( nTbl )
   FOR i := 1 TO n
      cName := AllTrim( hbo_FieldName( nTbl, i ) )
      hMeta := ORM_TypeMeta( hbo_FieldType( nTbl, cName ), ;
                             hbo_FieldDecimals( nTbl, cName ) )
      /* O HIBRIDO: tenta a fonte de metadados RICA do servidor por campo --
         dicionario vivo Advantage (add://) OU catalogo SQL information_schema
         (postgresql://). REQUIRED -> nullable; DEFAULT -> default. Fallback
         gracioso: backend sem fonte rica (DBF livre) devolve "" -> tabela LIVRE
         (nullable, sem default). Um unico ponto, capacidade-agnostico. */
      lNull := ! DictRequired( oConn:Handle(), cTable, cName )
      xDef  := DictDefault( oConn:Handle(), cTable, cName )
      AAdd( aCols, { "nome" => cName, "cast" => hMeta[ "cast" ], "pk" => .F., ;
                     "nullable" => lNull, "default" => xDef, ;
                     "readonly" => hMeta[ "readonly" ], ;
                     "rule"    => DictRule( oConn:Handle(), cTable, cName ), ;
                     "msg"     => DictMsg( oConn:Handle(), cTable, cName ), ;
                     "comment" => DictComment( oConn:Handle(), cTable, cName ) } )
      IF Lower( cName ) == "id"
         lHasId := .T.
      ENDIF
   NEXT
   hbo_TableClose( nTbl )
   /* PK: 'id' se existir, senao a 1a coluna */
   IF Len( aCols ) > 0
      IF lHasId
         FOR EACH c IN aCols
            IF Lower( c[ "nome" ] ) == "id"
               c[ "pk" ] := .T.
               EXIT
            ENDIF
         NEXT
      ELSE
         aCols[ 1 ][ "pk" ] := .T.
      ENDIF
   ENDIF
   RETURN aCols

/* tag de campo (+ decimais) -> { cast, readonly }.
   Cobre os tipos estendidos do catalogo ADS/DBF alem do basico C/N/D/L:
   '+' autoinc, '=' modtime, '^' rowversion = geridos pela engine (read-only);
   'Y' money = inteiro 64 de 4 decimais (gravavel); 'B' double; 'I/2/4/8' inteiros. */
FUNCTION ORM_TypeMeta( cTag, nDec )
   LOCAL cCast := "string", lRO := .F.
   hb_default( @nDec, 0 )
   DO CASE
   CASE cTag == "N" ; cCast := DecOrInt( nDec )
   CASE cTag == "C" ; cCast := "string"
   CASE cTag == "M" ; cCast := "string"
   CASE cTag == "L" ; cCast := "boolean"
   CASE cTag == "D" ; cCast := "date"
   CASE cTag == "T" .OR. cTag == "@" ; cCast := "datetime"
   CASE cTag == "+" ; cCast := "integer"  ; lRO := .T.   // autoincremento
   CASE cTag == "=" ; cCast := "datetime" ; lRO := .T.   // carimbo de modificacao
   CASE cTag == "^" ; cCast := "integer"  ; lRO := .T.   // versao de linha
   CASE cTag == "Y" ; cCast := "decimal:4"               // moeda
   CASE cTag == "B" ; cCast := "decimal"                 // ponto flutuante
   CASE cTag == "I" .OR. cTag == "2" .OR. cTag == "4" .OR. cTag == "8" ; cCast := "integer"
   ENDCASE
   RETURN { "cast" => cCast, "readonly" => lRO }

STATIC FUNCTION DecOrInt( nDec )
   RETURN iif( nDec > 0, "decimal:" + LTrim( Str( Int( nDec ) ) ), "integer" )

/* { nome => lNullable } para toda coluna */
FUNCTION ORM_NullableFromCols( aCols )
   LOCAL h := hb_Hash(), c
   FOR EACH c IN aCols
      h[ c[ "nome" ] ] := hb_HGetDef( c, "nullable", .T. )
   NEXT
   RETURN h

/* { nome => valor-default } SO para colunas que declaram um default
   (chave ausente = sem default) */
FUNCTION ORM_DefaultsFromCols( aCols )
   LOCAL h := hb_Hash(), c
   FOR EACH c IN aCols
      IF hb_HGetDef( c, "default", NIL ) != NIL
         h[ c[ "nome" ] ] := c[ "default" ]
      ENDIF
   NEXT
   RETURN h

/* { nome => regra } SO para colunas com regra de validacao no dicionario */
FUNCTION ORM_RulesFromCols( aCols )
   LOCAL h := hb_Hash(), c
   FOR EACH c IN aCols
      IF hb_HGetDef( c, "rule", NIL ) != NIL
         h[ c[ "nome" ] ] := c[ "rule" ]
      ENDIF
   NEXT
   RETURN h

/* { nome => mensagem } SO para colunas com mensagem de validacao */
FUNCTION ORM_MsgsFromCols( aCols )
   LOCAL h := hb_Hash(), c
   FOR EACH c IN aCols
      IF hb_HGetDef( c, "msg", NIL ) != NIL
         h[ c[ "nome" ] ] := c[ "msg" ]
      ENDIF
   NEXT
   RETURN h

/* { nome => comentario } SO para colunas com comentario/descricao */
FUNCTION ORM_CommentsFromCols( aCols )
   LOCAL h := hb_Hash(), c
   FOR EACH c IN aCols
      IF hb_HGetDef( c, "comment", NIL ) != NIL
         h[ c[ "nome" ] ] := c[ "comment" ]
      ENDIF
   NEXT
   RETURN h

/* { nome, ... } das colunas geridas pela engine (nao gravaveis) */
FUNCTION ORM_ReadOnlyFromCols( aCols )
   LOCAL a := {}, c
   FOR EACH c IN aCols
      IF hb_HGetDef( c, "readonly", .F. )
         AAdd( a, c[ "nome" ] )
      ENDIF
   NEXT
   RETURN a

FUNCTION ORM_CastsFromCols( aCols )
   LOCAL h := hb_Hash(), c
   FOR EACH c IN aCols
      h[ c[ "nome" ] ] := c[ "cast" ]
   NEXT
   RETURN h

FUNCTION ORM_PkFromCols( aCols )
   LOCAL c
   FOR EACH c IN aCols
      IF c[ "pk" ]
         RETURN c[ "nome" ]
      ENDIF
   NEXT
   RETURN "id"

/* ---- estrategia SQL: catalogo declarado via PRAGMA table_info ------------ */
STATIC FUNCTION IntrospectSql( oConn, cTable )
   LOCAL aCols := {}, aRows, hR, cIdent
   cIdent := TORMGrammar():New():QuoteIdent( cTable )    // valida; aborta se junk
   /* preferir o parametro vinculado; cair p/ a forma de comando com
      identificador JA validado (sem valor cru) se o passthrough nao popular */
   aRows := oConn:Query( "SELECT name, type, pk, notnull, dflt_value " + ;
                         "FROM pragma_table_info(:p1)", { { "p1", cTable } } )
   IF Len( aRows ) == 0
      aRows := oConn:Query( "PRAGMA table_info(" + cIdent + ")" )
   ENDIF
   IF Len( aRows ) == 0
      IntrospectRaise( "tabela sem catalogo" )
   ENDIF
   FOR EACH hR IN aRows
      AAdd( aCols, { ;
         "nome"     => AllTrim( hb_HGetDef( hR, "name", "" ) ), ;
         "cast"     => SqlTypeToken( hb_HGetDef( hR, "type", "" ) ), ;
         "pk"       => ( Val( hb_CStr( hb_HGetDef( hR, "pk", "0" ) ) ) > 0 ), ;
         "nullable" => ( Val( hb_CStr( hb_HGetDef( hR, "notnull", "0" ) ) ) == 0 ), ;
         "default"  => SqlDefaultVal( hb_HGetDef( hR, "dflt_value", NIL ) ), ;
         "readonly" => .F. } )
   NEXT
   RETURN aCols

/* dflt_value do PRAGMA: NULL/vazio = sem default (NIL); senao o literal */
STATIC FUNCTION SqlDefaultVal( xDv )
   IF xDv == NIL .OR. ( HB_ISSTRING( xDv ) .AND. Empty( xDv ) )
      RETURN NIL
   ENDIF
   RETURN xDv

/* tipo declarado (texto) -> token de cast. Ordem: DATETIME antes de DATE. */
STATIC FUNCTION SqlTypeToken( cDecl )
   LOCAL cU := Upper( AllTrim( hb_CStr( cDecl ) ) )
   DO CASE
   CASE "INT" $ cU                                  ; RETURN "integer"
   CASE "BOOL" $ cU                                 ; RETURN "boolean"
   CASE "DATETIME" $ cU .OR. "TIMESTAMP" $ cU       ; RETURN "datetime"
   CASE "DATE" $ cU                                 ; RETURN "date"
   CASE "DEC" $ cU .OR. "NUMERIC" $ cU              ; RETURN DecToken( cU )
   CASE "REAL" $ cU .OR. "FLOA" $ cU .OR. "DOUB" $ cU ; RETURN "decimal"
   ENDCASE
   RETURN "string"

/* "DECIMAL(12,2)" -> "decimal:2"; sem escala util -> "decimal" */
STATIC FUNCTION DecToken( cU )
   LOCAL nOpen := At( "(", cU ), cInner, aPS, nScale
   IF nOpen == 0
      RETURN "decimal"
   ENDIF
   cInner := StrTran( SubStr( cU, nOpen + 1 ), ")", "" )
   aPS    := hb_ATokens( cInner, "," )
   IF Len( aPS ) >= 2
      nScale := Int( Val( aPS[ 2 ] ) )
      RETURN iif( nScale > 0, "decimal:" + LTrim( Str( nScale ) ), "decimal" )
   ENDIF
   RETURN "decimal"

/* REQUIRED do dicionario -> .T. se a propriedade for "verdadeira" (T/Y/1).
   A engine guarda a propriedade como string crua; convencao auto-consistente. */
STATIC FUNCTION DictRequired( nConn, cTable, cField )
   LOCAL c := AllTrim( hbo_DDGetFieldProp( nConn, cTable, cField, DD_FIELD_REQUIRED ) )
   RETURN ! Empty( c ) .AND. ( Upper( Left( c, 1 ) ) $ "TY1" )

/* DEFAULT do dicionario -> o literal declarado, ou NIL se ausente/vazio */
STATIC FUNCTION DictDefault( nConn, cTable, cField )
   LOCAL c := hbo_DDGetFieldProp( nConn, cTable, cField, DD_FIELD_DEFAULT )
   RETURN iif( Empty( c ), NIL, c )

/* regra de validacao do dicionario -> a expressao declarada, ou NIL */
STATIC FUNCTION DictRule( nConn, cTable, cField )
   LOCAL c := hbo_DDGetFieldProp( nConn, cTable, cField, DD_FIELD_VALIDATION_RULE )
   RETURN iif( Empty( c ), NIL, c )

/* mensagem de validacao do dicionario -> o texto declarado, ou NIL */
STATIC FUNCTION DictMsg( nConn, cTable, cField )
   LOCAL c := hbo_DDGetFieldProp( nConn, cTable, cField, DD_FIELD_VALIDATION_MSG )
   RETURN iif( Empty( c ), NIL, c )

/* comentario/descricao do dicionario -> o texto declarado, ou NIL */
STATIC FUNCTION DictComment( nConn, cTable, cField )
   LOCAL c := hbo_DDGetFieldProp( nConn, cTable, cField, DD_FIELD_COMMENT )
   RETURN iif( Empty( c ), NIL, c )

STATIC PROCEDURE IntrospectRaise( cWhy )
   LOCAL oErr := ErrorNew()
   oErr:Subsystem := "hb_orm"
   oErr:SubCode := 1010
   oErr:Severity := ES_ERROR
   oErr:Description := "introspeccao falhou: " + cWhy    // sem SQL/path
   oErr:Operation := "ORM_Introspect"
   oErr:CanRetry := .F.
   Eval( ErrorBlock(), oErr )
   RETURN
