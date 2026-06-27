/* navexec.prg -- executor NAVEGACIONAL: dirige um cursor de tabela (open/scan/
   seek/append/write/delete) quando a conexao nao fala SQL. Mesma AST da Fatia 4
   entra; rows (hb_Hash) saem -- a forma identica a Connection:Query, p/ Model
   nao mudar. Backend de prova: dbf://. */
#include "hborm.ch"
#include "error.ch"

/* contador de instrumentacao do seek -- prova que o fast-path dispara */
STATIC s_nSeeks := 0

/* contadores de instrumentacao do AOF (pushdown server-side) */
STATIC s_nAof        := 0   // quantas vezes SetAOF foi instalado e aceito
STATIC s_nAofScanned := 0   // quantas rows o cursor AOF percorreu (prova de reducao)

/* contadores de instrumentacao do caminho ordenado por indice */
STATIC s_nOrd        := 0   // quantas vezes NavSelectOrdered disparou (nao devolveu NIL)
STATIC s_nOrdWalked  := 0   // quantos registros foram visitados na ultima chamada ordenada

/* ---- DDL: cria DBF a partir do AST do Blueprint -------------------------- */
FUNCTION NavCreateTable( oConn, hAst )
   LOCAL cTab := TORMGrammar():New():QuoteIdent( hAst[ "table" ] )
   LOCAL cFields := NavFieldDef( hAst[ "columns" ] ), nTbl, hIdx, lOk := .T.
   LOCAL cCol, cExpr, aCols
   nTbl := hbo_CreateTable( oConn:Handle(), cTab, cFields )
   IF nTbl == 0
      RETURN .F.
   ENDIF
   /* indices declarados -> tags CDX (uma coluna por tag; chave = nome do campo) */
   FOR EACH hIdx IN hAst[ "indexes" ]
      aCols := hIdx[ "columns" ]
      IF aCols != NIL .AND. Len( aCols ) > 0
         cCol  := TORMGrammar():New():QuoteIdent( aCols[ 1 ] )
         cExpr := Upper( cCol )                       // DBF guarda campo em CAIXA ALTA
         IF ! hbo_CreateIndex( nTbl, Left( "X" + cCol, 10 ), cExpr )
            lOk := .F.
         ENDIF
      ENDIF
   NEXT
   hbo_TableClose( nTbl )
   RETURN lOk

/* AST de colunas -> string ACE "NOME,TIPO,LARG,DEC;...". Tipos xBase seguros:
   integer/id -> N,10,0 (PK assinada pelo ORM); decimal -> N,prec,scale;
   string -> C,len; text -> M; boolean -> L; date -> D; datetime -> C,19 (ISO). */
STATIC FUNCTION NavFieldDef( aColumns )
   LOCAL c := "", hCol, cName, cType, nLen, nDec, cPart
   FOR EACH hCol IN aColumns
      cName := Upper( hCol[ "name" ] )
      cType := hCol[ "type" ]
      DO CASE
      CASE cType == "integer"  ; cPart := cName + ",N,10,0"
      CASE cType == "decimal"
         nLen := hb_HGetDef( hCol, "prec", 12 )
         nDec := hb_HGetDef( hCol, "scale", 2 )
         cPart := cName + ",N," + LTrim( Str( nLen ) ) + "," + LTrim( Str( nDec ) )
      CASE cType == "string"
         nLen := hb_HGetDef( hCol, "len", 100 )
         cPart := cName + ",C," + LTrim( Str( nLen ) )
      CASE cType == "text"     ; cPart := cName + ",M,10"
      CASE cType == "boolean"  ; cPart := cName + ",L,1"
      CASE cType == "date"     ; cPart := cName + ",D,8"
      CASE cType == "datetime" ; cPart := cName + ",C,19"
      CASE cType == "json"     ; cPart := cName + ",M,10"
      OTHERWISE                ; cPart := cName + ",C,100"
      ENDCASE
      c += iif( Empty( c ), "", ";" ) + cPart
   NEXT
   RETURN c

STATIC PROCEDURE NavRaise( cOp, cDesc )
   LOCAL oErr := ErrorNew()
   oErr:Subsystem := "hb_orm"
   oErr:SubCode := 1200
   oErr:Severity := ES_ERROR
   oErr:Description := cDesc                           // sem SQL/path
   oErr:Operation := cOp
   oErr:CanRetry := .F.
   Eval( ErrorBlock(), oErr )
   RETURN

/* thin PUBLIC wrapper so external callers (model.prg) can raise a nav error
   without exposing the STATIC NavRaise directly */
FUNCTION NavUnsupported( cOp, cDesc )
   NavRaise( cOp, cDesc )
   RETURN NIL

/* wrapper PUBLICO da hidratacao por-registro (a STATIC NavHydrateRow ja e
   provada na Fatia 9); permite ao TORMCursor reusar sem duplicar a logica. */
FUNCTION NavHydrateRowPub( nTbl, aNames )
   RETURN NavHydrateRow( nTbl, aNames )

/* InsertMany navegacional: executa NavInsert por linha (sem ON CONFLICT).
   Devolve .T. se todas as linhas foram inseridas, .F. se alguma falhou ou
   a lista estava vazia. */
FUNCTION NavInsertMany( oConn, cTable, aRows )
   LOCAL hRow, lOk := .T.
   IF aRows == NIL .OR. Len( aRows ) == 0
      RETURN .F.
   ENDIF
   FOR EACH hRow IN aRows
      lOk := ( NavInsert( oConn, cTable, hRow, "id" ) != NIL ) .AND. lOk
   NEXT
   RETURN lOk

/* ---- INSERT navegacional: append + set por tipo + write ------------------ */
FUNCTION NavInsert( oConn, cTable, hAttrs, cPk )
   LOCAL cTab := TORMGrammar():New():QuoteIdent( cTable )
   LOCAL nTbl, cK, xPk, lOk := .T.
   IF cPk == NIL ; cPk := "id" ; ENDIF
   /* PK: usa a fornecida; senao max+1 (autoinc deterministico, portavel).
      Resolvido ANTES de abrir o handle de escrita -- evita dois handles
      simultaneos na mesma tabela. */
   xPk := hb_HGetDef( hAttrs, cPk, NIL )
   IF xPk == NIL
      xPk := NavNextId( oConn, cTable, cPk )
   ENDIF
   nTbl := hbo_OpenTable( oConn:Handle(), cTab )
   IF nTbl == 0
      RETURN NIL
   ENDIF
   hbo_OpenIdxBag( nTbl )                             // mantem o(s) indice(s) no append
   IF ! hbo_Append( nTbl )
      hbo_TableClose( nTbl ) ; RETURN NIL
   ENDIF
   NavSetField( nTbl, Upper( cPk ), xPk )
   FOR EACH cK IN hb_HKeys( hAttrs )
      IF ! ( Lower( cK ) == Lower( cPk ) )
         lOk := NavSetField( nTbl, Upper( cK ), hAttrs[ cK ] ) .AND. lOk
      ENDIF
   NEXT
   lOk := hbo_WriteRec( nTbl ) .AND. lOk
   hbo_TableClose( nTbl )
   RETURN iif( lOk, xPk, NIL )

/* INSERT navegacional CRU: grava EXATAMENTE os campos de hRow, sem injetar PK.
   Para materializacao de result-set (o NavInsert injeta um id autoinc quando
   ausente, o que aqui seria errado). Assume que as chaves de hRow sao campos
   reais da tabela (result-set homogeneo). */
FUNCTION NavInsertRaw( oConn, cTable, hRow )
   LOCAL cTab := TORMGrammar():New():QuoteIdent( cTable )
   LOCAL nTbl, cK, lOk := .T.
   nTbl := hbo_OpenTable( oConn:Handle(), cTab )
   IF nTbl == 0
      RETURN .F.
   ENDIF
   hbo_OpenIdxBag( nTbl )                              // mantem indice(s) no append
   IF ! hbo_Append( nTbl )
      hbo_TableClose( nTbl ) ; RETURN .F.
   ENDIF
   FOR EACH cK IN hb_HKeys( hRow )
      lOk := NavSetField( nTbl, Upper( cK ), hRow[ cK ] ) .AND. lOk
   NEXT
   lOk := hbo_WriteRec( nTbl ) .AND. lOk
   hbo_TableClose( nTbl )
   RETURN lOk

/* proximo id = (maior id atual) + 1; 1 em tabela vazia. Scan simples. */
STATIC FUNCTION NavNextId( oConn, cTable, cPk )
   LOCAL nTbl, nMax := 0, xV
   nTbl := hbo_OpenTable( oConn:Handle(), TORMGrammar():New():QuoteIdent( cTable ) )
   IF nTbl == 0
      RETURN 1
   ENDIF
   hbo_GoTop( nTbl )
   DO WHILE ! hbo_Eof( nTbl )
      xV := hbo_GetNum( nTbl, Upper( cPk ) )
      IF xV > nMax ; nMax := xV ; ENDIF
      hbo_Skip( nTbl, 1 )
   ENDDO
   hbo_TableClose( nTbl )
   RETURN nMax + 1

/* set despachado pelo TIPO HARBOUR do valor (numero/logico/data/string) */
STATIC FUNCTION NavSetField( nTbl, cField, xVal )
   DO CASE
   CASE xVal == NIL          ; RETURN .T.            // deixa default/vazio
   CASE HB_ISNUMERIC( xVal ) ; RETURN hbo_SetNum( nTbl, cField, xVal )
   CASE HB_ISLOGICAL( xVal ) ; RETURN hbo_SetLog( nTbl, cField, xVal )
   CASE HB_ISDATE( xVal )    ; RETURN hbo_SetStr( nTbl, cField, DToS( xVal ) )
   ENDCASE
   RETURN hbo_SetStr( nTbl, cField, hb_CStr( xVal ) )

/* ---- UPDATE / DELETE navegacional: localiza a PK e grava/apaga ----------- */
FUNCTION NavUpdate( oConn, cTable, cPk, xPk, hAttrs )
   LOCAL cTab := TORMGrammar():New():QuoteIdent( cTable )
   LOCAL nTbl, cK, lOk := .T.
   IF cPk == NIL ; cPk := "id" ; ENDIF
   nTbl := hbo_OpenTable( oConn:Handle(), cTab )
   IF nTbl == 0 ; RETURN .F. ; ENDIF
   hbo_OpenIdxBag( nTbl )                             // mantem indice(s) na regravacao
   IF ! NavLocate( nTbl, cPk, xPk )
      hbo_TableClose( nTbl ) ; RETURN .F.
   ENDIF
   FOR EACH cK IN hb_HKeys( hAttrs )
      IF ! ( Lower( cK ) == Lower( cPk ) )            // nao reescreve a PK
         lOk := NavSetField( nTbl, Upper( cK ), hAttrs[ cK ] ) .AND. lOk
      ENDIF
   NEXT
   lOk := hbo_WriteRec( nTbl ) .AND. lOk
   hbo_TableClose( nTbl )
   RETURN lOk

FUNCTION NavDelete( oConn, cTable, cPk, xPk )
   LOCAL cTab := TORMGrammar():New():QuoteIdent( cTable )
   LOCAL nTbl, lOk
   IF cPk == NIL ; cPk := "id" ; ENDIF
   nTbl := hbo_OpenTable( oConn:Handle(), cTab )
   IF nTbl == 0 ; RETURN .F. ; ENDIF
   hbo_OpenIdxBag( nTbl )                             // mantem indice(s) na exclusao
   IF ! NavLocate( nTbl, cPk, xPk )
      hbo_TableClose( nTbl ) ; RETURN .F.
   ENDIF
   lOk := hbo_DeleteRec( nTbl )
   hbo_TableClose( nTbl )
   RETURN lOk

/* posiciona o cursor no registro cuja PK == xPk (scan; seek vem na Task 6) */
STATIC FUNCTION NavLocate( nTbl, cPk, xPk )
   LOCAL cField := Upper( cPk )
   hbo_GoTop( nTbl )
   DO WHILE ! hbo_Eof( nTbl )
      IF hbo_GetNum( nTbl, cField ) == xPk
         RETURN .T.
      ENDIF
      hbo_Skip( nTbl, 1 )
   ENDDO
   RETURN .F.

/* ---- SELECT navegacional: seek (fast path) | AOF | scan -> filtra -> ordena -> fatia
   O resultado e IDENTICO em todos os caminhos -- seek/AOF sao otimizacoes;
   NavFilter(aWheres COMPLETO) SEMPRE roda apos o scan (correcao nao depende do AOF). */
FUNCTION NavSelect( oConn, hAst )
   RETURN NavSelectImpl( oConn, hAst, .T. )

/* oraculo de teste / desligador: forca o caminho scan+filtro sem AOF */
FUNCTION NavSelectNoAof( oConn, hAst )
   RETURN NavSelectImpl( oConn, hAst, .F. )

STATIC FUNCTION NavSelectImpl( oConn, hAst, lUseAof )
   LOCAL aRows, aWheres, cCond, aOrders
   NavGuardUnsupported( hAst )                        // join/group/having/agg/raw -> levanta
   aWheres := hb_HGetDef( hAst, "wheres", {} )
   /* fast path: 1 termo simples "=" + indice na coluna -> seek O(log n) */
   aRows := NavTrySeek( oConn, hAst[ "table" ], aWheres )
   IF aRows == NIL                                    // nao aplicavel / sem indice -> ordered | AOF | scan
      /* caminho ordenado: 1 coluna indexada + lUseAof (oracle usa .F. para bypass) */
      IF lUseAof
         aOrders := hb_HGetDef( hAst, "orders", {} )
         IF Len( aOrders ) == 1
            aRows := NavSelectOrdered( oConn, hAst, aOrders[ 1 ] )
            IF aRows != NIL
               RETURN aRows                           // ja filtrado+ordenado+limitado
            ENDIF
         ENDIF
      ENDIF
      aRows := NIL
      IF lUseAof
         cCond := NavBuildAof( aWheres )
         IF cCond != NIL
            aRows := NavScanRowsAof( oConn, hAst[ "table" ], cCond )  // NIL se SetAOF falhou
         ENDIF
      ENDIF
      IF aRows == NIL                                 // sem push OU SetAOF falhou -> fallback scan
         aRows := NavScanRows( oConn, hAst[ "table" ] )
      ENDIF
      aRows := NavFilter( aRows, aWheres )            // SEMPRE o conjunto where COMPLETO
   ENDIF
   aRows := NavApplySoftDelete( aRows, hb_HGetDef( hAst, "softDelete", NIL ) )
   aRows := NavApplyOrder( aRows, hb_HGetDef( hAst, "orders", {} ) )
   aRows := NavApplyLimit( aRows, hb_HGetDef( hAst, "limit", NIL ), ;
                                  hb_HGetDef( hAst, "offset", NIL ) )
   RETURN aRows

/* caminho ordenado: navega na ordem do indice (1 col indexada, ASC/DESC),
   com bounds OrdScope do range na col ordenada + parada-cedo no limit.
   Devolve rows JA na ordem final, ou NIL (fallback). Aplica NavMatch completo
   + soft-delete por registro -> identico ao caminho atual. nav em nIdx, leitura
   em nTbl (compartilham registro corrente; passar nIdx ativa a ordem+escopo). */
STATIC FUNCTION NavSelectOrdered( oConn, hAst, aOrder )
   LOCAL cTab := TORMGrammar():New():QuoteIdent( hAst[ "table" ] )
   LOCAL cCol := aOrder[ 1 ], lDesc := ( Upper( aOrder[ 2 ] ) == "DESC" )
   LOCAL aWheres := hb_HGetDef( hAst, "wheres", {} )
   LOCAL hSoft := hb_HGetDef( hAst, "softDelete", NIL )
   LOCAL nLimit := hb_HGetDef( hAst, "limit", NIL )
   LOCAL nOffset := hb_HGetDef( hAst, "offset", NIL )
   LOCAL nTbl, nIdx, aNames := {}, n, i, aRows := {}, hRow
   LOCAL nWalked := 0, nMatched := 0, nSkip, nTake, xTop, xBot

   nTbl := hbo_OpenTable( oConn:Handle(), cTab )
   IF nTbl == 0
      RETURN NIL
   ENDIF
   nIdx := NavSeekIndexFor( nTbl, cCol )                 // 0 se nao ha tag p/ a coluna
   IF nIdx == 0
      hbo_TableClose( nTbl ) ; RETURN NIL                // sem indice -> fallback
   ENDIF
   /* bounds OrdScope dos termos AND na coluna ordenada (NIL se houver OR) */
   xTop := NavScopeBound( aWheres, cCol, .T. )
   xBot := NavScopeBound( aWheres, cCol, .F. )
   IF xTop != NIL .AND. ! NavSetScopeVal( nIdx, 0, xTop )
      hbo_ClearScope( nIdx, 0 ) ; hbo_ClearScope( nIdx, 1 )
      hbo_TableClose( nTbl ) ; RETURN NIL
   ENDIF
   IF xBot != NIL .AND. ! NavSetScopeVal( nIdx, 1, xBot )
      hbo_ClearScope( nIdx, 0 ) ; hbo_ClearScope( nIdx, 1 )
      hbo_TableClose( nTbl ) ; RETURN NIL
   ENDIF
   n := hbo_NumFields( nTbl )
   FOR i := 1 TO n ; AAdd( aNames, AllTrim( hbo_FieldName( nTbl, i ) ) ) ; NEXT
   nSkip := iif( nOffset == NIL, 0, nOffset )
   nTake := iif( nLimit == NIL, -1, nLimit )
   IF nTake == 0                                           // limit=0 -> oracle devolve {} -> paridade
      hbo_ClearScope( nIdx, 0 ) ; hbo_ClearScope( nIdx, 1 )
      hbo_TableClose( nTbl )
      s_nOrdWalked := 0 ; s_nOrd++
      RETURN {}
   ENDIF
   IF lDesc ; hbo_GoBottom( nIdx ) ; ELSE ; hbo_GoTop( nIdx ) ; ENDIF
   DO WHILE iif( lDesc, ! hbo_Bof( nIdx ), ! hbo_Eof( nIdx ) )
      nWalked++
      IF ! hbo_IsDeleted( nTbl )
         hRow := NavHydrateRow( nTbl, aNames )
         IF NavMatch( hRow, aWheres ) .AND. NavSoftAlive( hRow, hSoft )
            nMatched++
            IF nMatched > nSkip
               AAdd( aRows, hRow )
               IF nTake >= 0 .AND. Len( aRows ) >= nTake
                  EXIT                                    // parada-cedo
               ENDIF
            ENDIF
         ENDIF
      ENDIF
      hbo_Skip( nIdx, iif( lDesc, -1, 1 ) )
   ENDDO
   hbo_ClearScope( nIdx, 0 ) ; hbo_ClearScope( nIdx, 1 )
   hbo_TableClose( nTbl )
   s_nOrdWalked := nWalked
   s_nOrd++
   RETURN aRows

/* valor do bound p/ a coluna ordenada: lTop -> termo >=/> ; !lTop -> termo <=/< .
   So termos AND simple sobre cCol; se HOUVER QUALQUER OR na lista -> NIL (um
   bound parcial sob OR sub-incluiria). Apenas valores numericos/string (data
   nao vira bound em V1 -> filtrada por NavMatch). */
STATIC FUNCTION NavScopeBound( aWheres, cCol, lTop )
   LOCAL w, i, cOp, xVal
   FOR i := 2 TO Len( aWheres )
      IF Upper( hb_HGetDef( aWheres[ i ], "bool", "AND" ) ) == "OR"
         RETURN NIL
      ENDIF
   NEXT
   FOR EACH w IN aWheres
      IF HB_ISHASH( w ) .AND. hb_HGetDef( w, "kind", "simple" ) == "simple" ;
         .AND. Lower( hb_HGetDef( w, "col", "" ) ) == Lower( cCol )
         cOp  := hb_HGetDef( w, "op", "=" )
         xVal := hb_HGetDef( w, "val", NIL )
         IF ! ( HB_ISNUMERIC( xVal ) .OR. HB_ISSTRING( xVal ) )
            LOOP
         ENDIF
         IF lTop  .AND. ( cOp == ">=" .OR. cOp == ">" ) ; RETURN xVal ; ENDIF
         IF !lTop .AND. ( cOp == "<=" .OR. cOp == "<" ) ; RETURN xVal ; ENDIF
      ENDIF
   NEXT
   RETURN NIL

/* Dispatch pelo TIPO DO VALOR (numerico->SetScopeNum, else->SetScopeStr); tipo-errado mis-bounds mas NavMatch refina -> resultado correto */
STATIC FUNCTION NavSetScopeVal( nIdx, nScope, xVal )
   IF HB_ISNUMERIC( xVal )
      RETURN hbo_SetScopeNum( nIdx, nScope, xVal )
   ENDIF
   RETURN hbo_SetScopeStr( nIdx, nScope, hb_CStr( xVal ) )

/* vivo por soft-delete (mesma semantica do NavApplySoftDelete, por registro) */
STATIC FUNCTION NavSoftAlive( hRow, hSoft )
   LOCAL cKey, lNeg, lAlive
   IF hSoft == NIL
      RETURN .T.
   ENDIF
   cKey := Lower( hSoft[ "col" ] )
   lNeg := hb_HGetDef( hSoft, "negate", .F. )
   lAlive := Empty( hb_HGetDef( hRow, cKey, NIL ) )
   RETURN iif( lNeg, ! lAlive, lAlive )

/* ---- seek por indice: prova de disparo via contador --------------------- *
   s_nSeeks (declarado no topo do modulo) conta SO os seeks que de fato
   dispararam (indice achado + AdsSeek emitido) -- nao conta o caminho "nao
   aplicavel/sem indice" (que devolve NIL e cai p/ scan). Permite ao teste
   distinguir seek real de fallback. */
FUNCTION NavSeekCount()      ; RETURN s_nSeeks
FUNCTION NavResetSeekCount() ; s_nSeeks := 0 ; RETURN NIL

FUNCTION NavAofCount()       ; RETURN s_nAof
FUNCTION NavResetAofCount()  ; s_nAof := 0 ; s_nAofScanned := 0 ; RETURN NIL
FUNCTION NavAofLastScanned() ; RETURN s_nAofScanned

FUNCTION NavOrdCount()       ; RETURN s_nOrd
FUNCTION NavResetOrdCount()  ; s_nOrd := 0 ; s_nOrdWalked := 0 ; RETURN NIL
FUNCTION NavOrdLastWalked()  ; RETURN s_nOrdWalked

/* devolve as rows casadas por seek, ou NIL se nao aplicavel (=> usa scan).
   Coleta todos os registros com a chave (soft-seek + walk enquanto igual);
   p/ PK unica e uma row. Chave numerica -> DOUBLEKEY; string -> STRINGKEY. */
STATIC FUNCTION NavTrySeek( oConn, cTable, aWheres )
   LOCAL cTab := TORMGrammar():New():QuoteIdent( cTable )
   LOCAL w, cCol, xVal, nTbl, nIdx, aNames := {}, n, i, aRows := {}, lHit, hRow
   IF Len( aWheres ) != 1
      RETURN NIL
   ENDIF
   w := aWheres[ 1 ]
   IF ! HB_ISHASH( w ) .OR. hb_HGetDef( w, "kind", "" ) != "simple" ;  RETURN NIL ; ENDIF
   IF hb_HGetDef( w, "op", "=" ) != "=" ;  RETURN NIL ; ENDIF
   cCol := w[ "col" ] ; xVal := w[ "val" ]
   nTbl := hbo_OpenTable( oConn:Handle(), cTab )
   IF nTbl == 0 ;  RETURN NIL ; ENDIF
   nIdx := NavSeekIndexFor( nTbl, cCol )              // 0 se nao ha tag p/ a coluna
   IF nIdx == 0
      hbo_TableClose( nTbl ) ;  RETURN NIL            // sem indice -> sinaliza scan
   ENDIF
   /* nomes p/ hidratar */
   n := hbo_NumFields( nTbl )
   FOR i := 1 TO n ; AAdd( aNames, AllTrim( hbo_FieldName( nTbl, i ) ) ) ; NEXT
   /* aqui o seek REALMENTE dispara: conta */
   s_nSeeks++
   lHit := iif( HB_ISNUMERIC( xVal ), hbo_SeekNum( nIdx, xVal ), ;
                                      hbo_SeekStr( nIdx, hb_CStr( xVal ) ) )
   IF lHit .AND. hbo_IsFound( nTbl )
      /* walk enquanto a chave continua igual; compara o valor HIDRATADO da
         coluna (serve p/ chave numerica E string, e respeita NULL/casts).
         Registros logicamente deletados sao ignorados (indice xBase nao
         remove a chave na exclusao logica; SET DELETED filtra em GoTop/Skip
         do scan mas nao pos-seek -- checar aqui mantem consistencia). */
      DO WHILE ! hbo_Eof( nTbl )
         hRow := NavHydrateRow( nTbl, aNames )
         IF ! NavEq( hb_HGetDef( hRow, Lower( cCol ), NIL ), xVal )
            EXIT
         ENDIF
         IF ! hbo_IsDeleted( nTbl )
            AAdd( aRows, hRow )
         ENDIF
         hbo_Skip( nTbl, 1 )
      ENDDO
   ENDIF
   hbo_TableClose( nTbl )
   RETURN aRows

/* handle do tag cujo nome casa a convencao do NavCreateTable ("X"+coluna) */
STATIC FUNCTION NavSeekIndexFor( nTbl, cCol )
   RETURN hbo_GetIndex( nTbl, Left( "X" + TORMGrammar():New():QuoteIdent( cCol ), 10 ) )

/* operacoes que o cursor nao faz: erro honesto (sem stub fantasma) */
STATIC PROCEDURE NavGuardUnsupported( hAst )
   LOCAL w
   IF Len( hb_HGetDef( hAst, "joins",   {} ) ) > 0 ;  NavRaise( "NavSelect", "join nao suportado no backend navegacional" ) ; ENDIF
   IF Len( hb_HGetDef( hAst, "groups",  {} ) ) > 0 ;  NavRaise( "NavSelect", "groupBy nao suportado no backend navegacional" ) ; ENDIF
   IF Len( hb_HGetDef( hAst, "havings", {} ) ) > 0 ;  NavRaise( "NavSelect", "having nao suportado no backend navegacional" ) ; ENDIF
   IF hb_HGetDef( hAst, "aggregate", NIL ) != NIL  ;  NavRaise( "NavSelect", "agregado nao suportado no backend navegacional" ) ; ENDIF
   FOR EACH w IN hb_HGetDef( hAst, "wheres", {} )
      IF HB_ISHASH( w ) .AND. hb_HGetDef( w, "kind", "" ) == "raw"
         NavRaise( "NavSelect", "SQL cru nao suportado no backend navegacional" )
      ENDIF
   NEXT
   RETURN

/* abre a tabela, le todos os registros vivos como row-hash hidratada */
STATIC FUNCTION NavScanRows( oConn, cTable )
   LOCAL cTab := TORMGrammar():New():QuoteIdent( cTable )
   LOCAL nTbl, aNames := {}, n, i, aRows := {}
   nTbl := hbo_OpenTable( oConn:Handle(), cTab )
   IF nTbl == 0
      NavRaise( "NavSelect", "tabela nao abre (navegacional)" )
      RETURN {}
   ENDIF
   n := hbo_NumFields( nTbl )
   FOR i := 1 TO n
      AAdd( aNames, AllTrim( hbo_FieldName( nTbl, i ) ) )
   NEXT
   hbo_GoTop( nTbl )
   DO WHILE ! hbo_Eof( nTbl )
      IF ! hbo_IsDeleted( nTbl )                       // pula registros logicamente deletados (hbo_DeleteRec)
         AAdd( aRows, NavHydrateRow( nTbl, aNames ) )
      ENDIF
      hbo_Skip( nTbl, 1 )
   ENDDO
   hbo_TableClose( nTbl )
   RETURN aRows

/* scan com AOF instalado no MESMO handle que faz o GoTop/Skip.
   Devolve rows, ou NIL se hbo_SetAOF falhou (rc != 0) -- sinaliza fallback
   p/ scan completo. NAO checa OptLevel: um AOF full-scan funcionando reporta
   NONE; o que importa e que o cursor honra o filtro (GoTop/Skip andam so nas
   linhas que casam). Grava s_nAofScanned = Len(rows) como prova de reducao. */
STATIC FUNCTION NavScanRowsAof( oConn, cTable, cCond )
   LOCAL cTab := TORMGrammar():New():QuoteIdent( cTable )
   LOCAL nTbl, aNames := {}, n, i, aRows := {}
   nTbl := hbo_OpenTable( oConn:Handle(), cTab )
   IF nTbl == 0
      NavRaise( "NavSelect", "tabela nao abre (navegacional)" )
      RETURN {}
   ENDIF
   IF ! hbo_SetAOF( nTbl, cCond )
      hbo_ClearAOF( nTbl )
      hbo_TableClose( nTbl )
      RETURN NIL                                       // SetAOF falhou -> sinaliza fallback
   ENDIF
   n := hbo_NumFields( nTbl )
   FOR i := 1 TO n
      AAdd( aNames, AllTrim( hbo_FieldName( nTbl, i ) ) )
   NEXT
   hbo_GoTop( nTbl )
   DO WHILE ! hbo_Eof( nTbl )
      IF ! hbo_IsDeleted( nTbl )
         AAdd( aRows, NavHydrateRow( nTbl, aNames ) )
      ENDIF
      hbo_Skip( nTbl, 1 )
   ENDDO
   hbo_ClearAOF( nTbl )
   hbo_TableClose( nTbl )
   s_nAofScanned := Len( aRows )                      // prova de reducao server-side
   s_nAof++
   RETURN aRows

/* hidratacao com getters NATIVOS vivos: N->double real, NULL->NIL via AdsIsNull;
   logico via "T"; C/D/T ficam string (a camada Casts do Model coage D/T, igual ao
   caminho SQL). Chave da row em minuscula p/ casar com as colunas do dominio. */
/* Le um campo numerico de forma uniforme entre backends. O getter de double
   nativo e a fonte primaria (preserva o comportamento ja provado dos backends
   de arquivo). Alguns backends de cursor so expoem o valor como texto (o getter
   de double devolve 0 sem erro); nesse caso recorre-se ao texto bruto, que e
   confiavel em todos os backends. dec==0 -> inteiro. */
STATIC FUNCTION NavReadNum( nTbl, cName, nDec )
   LOCAL nVal := hbo_GetNum( nTbl, cName ), cRaw, nRaw
   IF nVal == 0
      cRaw := AllTrim( hbo_Field( nTbl, cName ) )
      IF ! Empty( cRaw )
         nRaw := Val( cRaw )
         IF nRaw != 0
            nVal := nRaw
         ENDIF
      ENDIF
   ENDIF
   RETURN iif( nDec == 0, Int( nVal ), nVal )

STATIC FUNCTION NavHydrateRow( nTbl, aNames )
   LOCAL hRow := hb_Hash(), cName, cKey, cTag, cRaw
   FOR EACH cName IN aNames
      cKey := Lower( cName )
      IF hbo_IsNull( nTbl, cName )
         hRow[ cKey ] := NIL
         LOOP
      ENDIF
      cTag := hbo_FieldType( nTbl, cName )
      DO CASE
      CASE cTag == "N"
         hRow[ cKey ] := NavReadNum( nTbl, cName, hbo_FieldDecimals( nTbl, cName ) )
      CASE cTag == "L" ; hRow[ cKey ] := ( Upper( Left( hbo_Field( nTbl, cName ), 1 ) ) $ "TY" )
      OTHERWISE
         cRaw := hbo_Field( nTbl, cName )
         hRow[ cKey ] := iif( cTag != "C" .AND. Empty( cRaw ), NIL, cRaw )
      ENDCASE
   NEXT
   RETURN hRow

/* ---- predicados em memoria sobre o envelope wheres da Fatia 4 ------------ */
STATIC FUNCTION NavFilter( aRows, aWheres )
   LOCAL aOut := {}, hRow
   IF aWheres == NIL .OR. Len( aWheres ) == 0
      RETURN aRows
   ENDIF
   FOR EACH hRow IN aRows
      IF NavMatch( hRow, aWheres )
         AAdd( aOut, hRow )
      ENDIF
   NEXT
   RETURN aOut

/* esconde (ou isola) linhas apagadas por coluna soft-delete. hSoft NIL = no-op.
   vivo = coluna vazia/NIL; negate -> devolve so as apagadas. Top-level AND. */
STATIC FUNCTION NavApplySoftDelete( aRows, hSoft )
   LOCAL aOut := {}, hRow, cKey, lNeg, lAlive
   IF hSoft == NIL
      RETURN aRows
   ENDIF
   cKey := Lower( hSoft[ "col" ] )
   lNeg := hb_HGetDef( hSoft, "negate", .F. )
   FOR EACH hRow IN aRows
      lAlive := Empty( hb_HGetDef( hRow, cKey, NIL ) )
      IF iif( lNeg, ! lAlive, lAlive )
         AAdd( aOut, hRow )
      ENDIF
   NEXT
   RETURN aOut

/* combina os termos por bool (AND/OR), na ordem -- equivalente ao SQL plano */
STATIC FUNCTION NavMatch( hRow, aWheres )
   LOCAL lAcc := .T., lFirst := .T., w, lTerm, cBool, cKind
   FOR EACH w IN aWheres
      cKind := hb_HGetDef( w, "kind", "simple" )
      cBool := hb_HGetDef( w, "bool", "AND" )
      DO CASE
      CASE cKind == "simple"
         lTerm := NavCmp( hb_HGetDef( hRow, Lower( w[ "col" ] ), NIL ), w[ "op" ], w[ "val" ] )
      CASE cKind == "in"
         lTerm := ( AScan( w[ "vals" ], ;
                    {| x | NavEq( hb_HGetDef( hRow, Lower( w[ "col" ] ), NIL ), x ) } ) > 0 )
      CASE cKind == "null"

         /* Empty() trata campo em branco E chave ausente (NIL) como nulo -- semantica strict-null do nav */
         lTerm := iif( hb_HGetDef( w, "negate", .F. ), ;
                       ! Empty( hb_HGetDef( hRow, Lower( w[ "col" ] ), NIL ) ), ;
                          Empty( hb_HGetDef( hRow, Lower( w[ "col" ] ), NIL ) ) )
      OTHERWISE
         lTerm := .F.
      ENDCASE
      IF lFirst
         lAcc := lTerm ; lFirst := .F.
      ELSEIF cBool == "OR"
         lAcc := lAcc .OR. lTerm
      ELSE
         lAcc := lAcc .AND. lTerm
      ENDIF
   NEXT
   RETURN lAcc

STATIC FUNCTION NavEq( xA, xB )
   IF xA == NIL .OR. xB == NIL
      RETURN ( xA == NIL .AND. xB == NIL )
   ENDIF
   IF HB_ISNUMERIC( xA ) .AND. HB_ISNUMERIC( xB ) ; RETURN xA == xB ; ENDIF
   RETURN hb_CStr( xA ) == hb_CStr( xB )

STATIC FUNCTION NavCmp( xVal, cOp, xRef )
   DO CASE
   CASE cOp == "="  .OR. cOp == "=="  ; RETURN NavEq( xVal, xRef )
   CASE cOp == "!=" .OR. cOp == "<>"  ; RETURN ! NavEq( xVal, xRef )
   ENDCASE
   IF xVal == NIL .OR. xRef == NIL
      RETURN .F.
   ENDIF
   DO CASE
   CASE cOp == "<"  ; RETURN xVal <  xRef
   CASE cOp == "<=" ; RETURN xVal <= xRef
   CASE cOp == ">"  ; RETURN xVal >  xRef
   CASE cOp == ">=" ; RETURN xVal >= xRef
   CASE Upper( cOp ) == "LIKE"
      // % wildcards stripped; pattern becomes a case-insensitive contains match
      RETURN ( StrTran( Upper( hb_CStr( xRef ) ), "%", "" ) $ Upper( hb_CStr( xVal ) ) )
   ENDCASE
   RETURN .F.

/* ordena por colunas {col,dir}; estavel o suficiente p/ o smoke (sort em copia) */
STATIC FUNCTION NavApplyOrder( aRows, aOrders )
   LOCAL aOut, a, cCol, lDesc
   IF aOrders == NIL .OR. Len( aOrders ) == 0
      RETURN aRows
   ENDIF
   aOut := AClone( aRows )
   FOR EACH a IN aOrders DESCEND                       // estabiliza multi-coluna (ultima 1o)
      cCol  := Lower( a[ 1 ] )
      lDesc := ( Upper( a[ 2 ] ) == "DESC" )
      ASort( aOut,,, {| x, y | NavOrderBlk( x, y, cCol, lDesc ) } )
   NEXT
   RETURN aOut

STATIC FUNCTION NavOrderBlk( x, y, cCol, lDesc )
   LOCAL xa := hb_HGetDef( x, cCol, NIL ), xb := hb_HGetDef( y, cCol, NIL )
   IF xa == NIL ; RETURN ! lDesc ; ENDIF
   IF xb == NIL ; RETURN lDesc ; ENDIF
   RETURN iif( lDesc, xa > xb, xa < xb )

STATIC FUNCTION NavApplyLimit( aRows, nLimit, nOffset )
   LOCAL aOut := {}, i, nFrom, nTo
   IF nLimit == NIL .AND. nOffset == NIL
      RETURN aRows
   ENDIF
   nFrom := iif( nOffset == NIL, 1, nOffset + 1 )
   nTo   := iif( nLimit == NIL, Len( aRows ), nFrom + nLimit - 1 )
   FOR i := nFrom TO Min( nTo, Len( aRows ) )
      AAdd( aOut, aRows[ i ] )
   NEXT
   RETURN aOut

/* ---- AOF: traduz o envelope wheres em condicao ADS empurravel ------------
   Devolve cCond (string) ou NIL quando nada seguro a empurrar. A correcao
   nao depende disto: NavSelect roda NavFilter(aWheres COMPLETO) apos o scan.
   Regra: so empurra C tal que predicado-completo => C (ver spec sec.4). */
FUNCTION NavBuildAof( aWheres )
   LOCAL lAnyOr := .F., w, aPush := {}, cCond, cTerm, i

   IF aWheres == NIL .OR. Len( aWheres ) == 0
      RETURN NIL
   ENDIF
   /* ha algum OR na lista (do 2o termo em diante)? */
   FOR i := 2 TO Len( aWheres )
      IF Upper( hb_HGetDef( aWheres[ i ], "bool", "AND" ) ) == "OR"
         lAnyOr := .T.
         EXIT
      ENDIF
   NEXT

   IF lAnyOr
      /* OR: so empurra se TODOS os termos forem AOF-able; reproduz o fold
         esquerda->direita do NavMatch com parenteses explicitos. */
      FOR EACH w IN aWheres
         IF ! NavAofAble( w )
            RETURN NIL
         ENDIF
      NEXT
      cCond := NavAofTerm( aWheres[ 1 ] )
      FOR i := 2 TO Len( aWheres )
         cTerm := NavAofTerm( aWheres[ i ] )
         cCond := "( " + cCond + ;
            iif( Upper( aWheres[ i ][ "bool" ] ) == "OR", " .OR. ", " .AND. " ) + ;
            cTerm + " )"
      NEXT
      RETURN cCond
   ENDIF

   /* so-AND: empurra os AOF-able; os nao-AOF ficam pro NavFilter completo */
   FOR EACH w IN aWheres
      IF NavAofAble( w )
         AAdd( aPush, NavAofTerm( w ) )
      ENDIF
   NEXT
   IF Len( aPush ) == 0
      RETURN NIL
   ENDIF
   cCond := ""
   FOR i := 1 TO Len( aPush )
      cCond += iif( i == 1, "", " .AND. " ) + aPush[ i ]
   NEXT
   RETURN cCond

/* um termo e empurravel? */
STATIC FUNCTION NavAofAble( w )
   LOCAL cKind, cOp, xVal, x
   IF ! HB_ISHASH( w )
      RETURN .F.
   ENDIF
   cKind := hb_HGetDef( w, "kind", "simple" )
   DO CASE
   CASE cKind == "simple"
      cOp  := hb_HGetDef( w, "op", "=" )
      xVal := hb_HGetDef( w, "val", NIL )
      /* op no conjunto exato (AScan, nao $ -- "=" e substring de "<=") */
      IF AScan( { "=", "==", "!=", "<>", "<", "<=", ">", ">=" }, cOp ) == 0
         RETURN .F.
      ENDIF
      /* data NAO e AOF-able (CTOD e funcao, fora do subset) */
      IF HB_ISDATE( xVal )
         RETURN .F.
      ENDIF
      /* numerico: todos os 8 ops sao AOF-able (comparacao identica no ADS e no NavMatch) */
      IF HB_ISNUMERIC( xVal )
         RETURN .T.
      ENDIF
      /* string: SÓ = e == sao AOF-able -- semantica safe (superset do NavMatch):
         ADS char = faz prefix-match (superset) -> NavFilter refina -> correto.
         Para !=/<>/</<=/>/>=: ADS padding/collation NAO coincide com o raw-byte
         NavMatch -> pode sub-incluir rows (under-inclusion) -> NAO empurrar;
         cai no NavFilter completo (safe fallback). */
      IF HB_ISSTRING( xVal )
         RETURN ( cOp == "=" .OR. cOp == "==" )
      ENDIF
      RETURN .F.
   CASE cKind == "in"
      FOR EACH x IN hb_HGetDef( w, "vals", {} )
         IF ! ( HB_ISNUMERIC( x ) .OR. HB_ISSTRING( x ) )
            RETURN .F.
         ENDIF
      NEXT
      RETURN Len( hb_HGetDef( w, "vals", {} ) ) > 0
   ENDCASE
   RETURN .F.

/* renderiza um termo AOF-able em string ADS (assume NavAofAble ja validou) */
STATIC FUNCTION NavAofTerm( w )
   LOCAL cKind := hb_HGetDef( w, "kind", "simple" ), cField, cOp, aOut, x
   cField := Upper( w[ "col" ] )
   IF cKind == "in"
      aOut := {}
      FOR EACH x IN w[ "vals" ]
         AAdd( aOut, NavAofLit( x ) )
      NEXT
      RETURN cField + " IN (" + ArrJoin( aOut, ", " ) + ")"
   ENDIF
   cOp := NavAofOp( w[ "op" ] )
   RETURN cField + " " + cOp + " " + NavAofLit( w[ "val" ] )

/* normaliza operador p/ a forma que o parser AOF aceita */
STATIC FUNCTION NavAofOp( cOp )
   DO CASE
   CASE cOp == "=="  ; RETURN "="
   CASE cOp == "<>"  ; RETURN "!="
   ENDCASE
   RETURN cOp

/* literal: numero cru (hb_ntos preserva precisao propria do valor -- nao depende
   de SET DECIMALS, evita arredondamento de Str() que tornaria o AOF mais restrito
   que o predicado real do NavMatch); string entre aspas simples com escape de aspas */
STATIC FUNCTION NavAofLit( xVal )
   IF HB_ISNUMERIC( xVal )
      RETURN hb_ntos( xVal )
   ENDIF
   RETURN "'" + StrTran( hb_CStr( xVal ), "'", "''" ) + "'"

/* join simples de array de strings (sem depender de hb_jsonEncode) */
STATIC FUNCTION ArrJoin( a, cSep )
   LOCAL c := "", i
   FOR i := 1 TO Len( a )
      c += iif( i == 1, "", cSep ) + a[ i ]
   NEXT
   RETURN c
