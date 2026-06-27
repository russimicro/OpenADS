/* hbo_ace.prg -- Harbour-callable glue over the ABI ACE entry points.
 *
 * The RDD ships Harbour wrappers for connect / SQL-statement / record-count,
 * but they report success while dropping the OUT statement and cursor handles
 * (they come back zero), so we call the ABI entry points directly here. Each
 * function is one ABI hop; cursor field reads use a single reusable buffer. */

#pragma BEGINDUMP

#include "hbapi.h"
#include "hbapiitm.h"
#include "openads/ace.h"
#include <string.h>
#include <ctype.h>

HB_FUNC( HBO_CONNECT )       /* ( cUri ) -> connection handle, 0 on failure */
{
   ADSHANDLE    hConn = 0;
   const char * szUri = hb_parc( 1 );
   UNSIGNED32   ulRc  = 1;
   /* Server type follows the URI scheme: tcp:// or tls:// are remote, every
      other form (sqlite://, a local directory, native files) is local. */
   UNSIGNED16   usType = 1;   /* ADS_LOCAL_SERVER */
   if( szUri &&
       ( strncmp( szUri, "tcp://", 6 ) == 0 || strncmp( szUri, "tls://", 6 ) == 0 ) )
      usType = 2;             /* ADS_REMOTE_SERVER */
   if( szUri )
      ulRc = AdsConnect60( ( UNSIGNED8 * ) szUri, usType,
                           NULL, NULL, 0, &hConn );
   hb_retnint( ulRc == 0 ? ( HB_MAXINT ) hConn : 0 );
}

HB_FUNC( HBO_DISCONNECT )    /* ( nConn ) -> .T./.F. */
{
   hb_retl( AdsDisconnect( ( ADSHANDLE ) hb_parnint( 1 ) ) == 0 );
}

HB_FUNC( HBO_SHOWDELETED )   /* ( lShow ) -> rc; .F. hides deleted rows (SET DELETED ON) */
{
   hb_retnl( ( long ) AdsShowDeleted( ( UNSIGNED16 ) ( hb_parl( 1 ) ? 1 : 0 ) ) );
}

/* Transação pela ABI -- SEED para o backend NATIVO/navegacional (ADS/DBF), onde
   AdsBeginTransaction opera na conexão. No passthrough SQL (sqlite) a ABI de
   transação devolve erro (não está ligada a esse backend); lá a Connection usa
   controle SQL-level (BEGIN/COMMIT/ROLLBACK via Execute). Estas entram em uso na
   fatia navegacional/dialetos. */
HB_FUNC( HBO_TXBEGIN )       /* ( nConn ) -> .T./.F. (rc==0) */
{
   hb_retl( AdsBeginTransaction( ( ADSHANDLE ) hb_parnint( 1 ) ) == 0 );
}

HB_FUNC( HBO_TXCOMMIT )      /* ( nConn ) -> .T./.F. */
{
   hb_retl( AdsCommitTransaction( ( ADSHANDLE ) hb_parnint( 1 ) ) == 0 );
}

HB_FUNC( HBO_TXROLLBACK )    /* ( nConn ) -> .T./.F. */
{
   hb_retl( AdsRollbackTransaction( ( ADSHANDLE ) hb_parnint( 1 ) ) == 0 );
}

HB_FUNC( HBO_INTX )          /* ( nConn ) -> .T./.F. */
{
   UNSIGNED16 bIn = 0;
   AdsInTransaction( ( ADSHANDLE ) hb_parnint( 1 ), &bIn );
   hb_retl( bIn != 0 );
}

HB_FUNC( HBO_VERSION )       /* () -> "major.minorletter" */
{
   UNSIGNED32 ulMajor = 0, ulMinor = 0;
   UNSIGNED8  ucLetter = 0;
   char       szDesc[ 128 ];
   UNSIGNED16 usLen = ( UNSIGNED16 ) sizeof( szDesc );
   char       szVer[ 32 ];
   AdsGetVersion( &ulMajor, &ulMinor, &ucLetter, ( UNSIGNED8 * ) szDesc, &usLen );
   hb_snprintf( szVer, sizeof( szVer ), "%u.%u%c",
                ( unsigned ) ulMajor, ( unsigned ) ulMinor, ( char ) ucLetter );
   hb_retc( szVer );
}

HB_FUNC( HBO_LASTERR )       /* () -> last error text */
{
   UNSIGNED32 ulCode = 0;
   char       szBuf[ 512 ];
   UNSIGNED16 usLen  = ( UNSIGNED16 ) sizeof( szBuf );
   AdsGetLastError( &ulCode, ( UNSIGNED8 * ) szBuf, &usLen );
   hb_retclen( szBuf, usLen );
}

HB_FUNC( HBO_STMTNEW )       /* ( nConn ) -> statement handle, 0 on failure */
{
   ADSHANDLE  hStmt = 0;
   UNSIGNED32 ulRc  = AdsCreateSQLStatement( ( ADSHANDLE ) hb_parnint( 1 ), &hStmt );
   hb_retnint( ulRc == 0 ? ( HB_MAXINT ) hStmt : 0 );
}

HB_FUNC( HBO_EXECDIRECT )    /* ( nStmt, cSql, @nCursor ) -> .T./.F. */
{
   ADSHANDLE    hCur  = 0;
   const char * szSql = hb_parc( 2 );
   UNSIGNED32   ulRc  = 1;
   if( szSql )
      ulRc = AdsExecuteSQLDirect( ( ADSHANDLE ) hb_parnint( 1 ),
                                  ( UNSIGNED8 * ) szSql, &hCur );
   hb_stornint( ( HB_MAXINT ) hCur, 3 );
   hb_retl( ulRc == 0 );
}

HB_FUNC( HBO_STMTCLOSE )     /* ( nStmt ) -> rc */
{
   hb_retnl( ( long ) AdsCloseSQLStatement( ( ADSHANDLE ) hb_parnint( 1 ) ) );
}

HB_FUNC( HBO_TABLECLOSE )    /* ( nCursor ) -> rc */
{
   hb_retnl( ( long ) AdsCloseTable( ( ADSHANDLE ) hb_parnint( 1 ) ) );
}

HB_FUNC( HBO_GOTOP )         /* ( nCursor ) -> rc */
{
   hb_retnl( ( long ) AdsGotoTop( ( ADSHANDLE ) hb_parnint( 1 ) ) );
}

HB_FUNC( HBO_SKIP )          /* ( nCursor, nRecs ) -> rc */
{
   hb_retnl( ( long ) AdsSkip( ( ADSHANDLE ) hb_parnint( 1 ),
                               ( SIGNED32 ) hb_parnl( 2 ) ) );
}

HB_FUNC( HBO_EOF )           /* ( nCursor ) -> .T./.F. */
{
   UNSIGNED16 usEof = 0;
   AdsAtEOF( ( ADSHANDLE ) hb_parnint( 1 ), &usEof );
   hb_retl( usEof != 0 );
}

HB_FUNC( HBO_GOBOTTOM )      /* ( nCursor ) -> rc */
{
   hb_retnl( ( long ) AdsGotoBottom( ( ADSHANDLE ) hb_parnint( 1 ) ) );
}

HB_FUNC( HBO_BOF )           /* ( nCursor ) -> .T./.F. */
{
   UNSIGNED16 usBof = 0;
   AdsAtBOF( ( ADSHANDLE ) hb_parnint( 1 ), &usBof );
   hb_retl( usBof != 0 );
}

HB_FUNC( HBO_NUMFIELDS )     /* ( nCursor ) -> field count */
{
   UNSIGNED16 usFields = 0;
   AdsGetNumFields( ( ADSHANDLE ) hb_parnint( 1 ), &usFields );
   hb_retni( ( int ) usFields );
}

HB_FUNC( HBO_FIELDNAME )     /* ( nCursor, nPos ) -> column name */
{
   char       szName[ 256 ];
   UNSIGNED16 usLen = ( UNSIGNED16 ) sizeof( szName );
   if( AdsGetFieldName( ( ADSHANDLE ) hb_parnint( 1 ),
                        ( UNSIGNED16 ) hb_parni( 2 ),
                        ( UNSIGNED8 * ) szName, &usLen ) == 0 )
      hb_retclen( szName, usLen );
   else
      hb_retc( "" );
}

HB_FUNC( HBO_FIELD )         /* ( nCursor, cName ) -> value (trimmed string) */
{
   ADSHANDLE  hCursor = ( ADSHANDLE ) hb_parnint( 1 );
   UNSIGNED32 ulLen   = 8192;
   char *     pBuf    = ( char * ) hb_xgrab( ulLen + 1 );
   UNSIGNED32 ulRc    = AdsGetField( hCursor, ( UNSIGNED8 * ) hb_parc( 2 ),
                                     ( UNSIGNED8 * ) pBuf, &ulLen, 0 );
   if( ulRc == 0 )
   {
      while( ulLen > 0 && pBuf[ ulLen - 1 ] == ' ' )
         ulLen--;
      hb_retclen( pBuf, ulLen );
   }
   else
      hb_retc( "" );
   hb_xfree( pBuf );
}

/* ---- navigational path (USE/APPEND/SEEK over native tables) -------------- *
 * For DBF/ADS and the navigational-only backends (pg/maria/odbc) the ORM
 * drives a real table cursor instead of emitting SQL: a scan honors SET
 * DELETED (which the SQL-over-xBase WHERE does not -- see findings F3), and
 * these are the only entry points reachable on the navigational-only
 * backends.  Numeric values must go through AdsSetDouble (the string encoder
 * left-justifies into a numeric field); logicals through AdsSetLogical. */

HB_FUNC( HBO_OPENTABLE )     /* ( nConn, cName ) -> table handle, 0 on failure */
{
   ADSHANDLE    hTable = 0;
   const char * szName = hb_parc( 2 );
   UNSIGNED32   ulRc   = 1;
   if( szName )
      ulRc = AdsOpenTable( ( ADSHANDLE ) hb_parnint( 1 ),
                           ( UNSIGNED8 * ) szName, NULL,
                           ADS_CDX, ADS_ANSI, ADS_PROPRIETARY_LOCKING,
                           ADS_IGNORERIGHTS, ADS_SHARED, &hTable );
   hb_retnint( ulRc == 0 ? ( HB_MAXINT ) hTable : 0 );
}

HB_FUNC( HBO_APPEND )        /* ( nTable ) -> .T./.F. */
{
   hb_retl( AdsAppendRecord( ( ADSHANDLE ) hb_parnint( 1 ) ) == 0 );
}

HB_FUNC( HBO_WRITEREC )      /* ( nTable ) -> .T./.F. */
{
   hb_retl( AdsWriteRecord( ( ADSHANDLE ) hb_parnint( 1 ) ) == 0 );
}

HB_FUNC( HBO_DELETEREC )     /* ( nTable ) -> .T./.F. */
{
   hb_retl( AdsDeleteRecord( ( ADSHANDLE ) hb_parnint( 1 ) ) == 0 );
}

HB_FUNC( HBO_ISDELETED )     /* ( nTable ) -> .T. if current record is deleted */
{
   UNSIGNED16 usDel = 0;
   AdsIsRecordDeleted( ( ADSHANDLE ) hb_parnint( 1 ), &usDel );
   hb_retl( usDel != 0 );
}

HB_FUNC( HBO_SETSTR )        /* ( nTable, cField, cVal ) -> .T./.F. */
{
   const char * szField = hb_parc( 2 );
   UNSIGNED32   ulRc    = 1;
   if( szField )
      ulRc = AdsSetString( ( ADSHANDLE ) hb_parnint( 1 ),
                           ( UNSIGNED8 * ) szField,
                           ( UNSIGNED8 * ) hb_parc( 3 ),
                           ( UNSIGNED32 ) hb_parclen( 3 ) );
   hb_retl( ulRc == 0 );
}

HB_FUNC( HBO_SETNUM )        /* ( nTable, cField, nVal ) -> .T./.F. */
{
   const char * szField = hb_parc( 2 );
   UNSIGNED32   ulRc    = 1;
   if( szField )
      ulRc = AdsSetDouble( ( ADSHANDLE ) hb_parnint( 1 ),
                           ( UNSIGNED8 * ) szField, hb_parnd( 3 ) );
   hb_retl( ulRc == 0 );
}

HB_FUNC( HBO_SETLOG )        /* ( nTable, cField, lVal ) -> .T./.F. */
{
   const char * szField = hb_parc( 2 );
   UNSIGNED32   ulRc    = 1;
   if( szField )
      ulRc = AdsSetLogical( ( ADSHANDLE ) hb_parnint( 1 ),
                            ( UNSIGNED8 * ) szField,
                            ( UNSIGNED16 ) ( hb_parl( 3 ) ? 1 : 0 ) );
   hb_retl( ulRc == 0 );
}

HB_FUNC( HBO_PREPARE )       /* ( nConn, cSql ) -> stmt handle, 0 on failure */
{
   ADSHANDLE    hStmt = 0;
   const char * szSql = hb_parc( 2 );
   UNSIGNED32   ulRc  = AdsCreateSQLStatement( ( ADSHANDLE ) hb_parnint( 1 ), &hStmt );
   if( ulRc == 0 && szSql )
      ulRc = AdsPrepareSQL( hStmt, ( UNSIGNED8 * ) szSql );
   if( ulRc != 0 && hStmt != 0 )
   {
      AdsCloseSQLStatement( hStmt );
      hStmt = 0;
   }
   hb_retnint( ulRc == 0 ? ( HB_MAXINT ) hStmt : 0 );
}

HB_FUNC( HBO_BINDSTR )       /* ( nStmt, cParam, cVal ) -> .T./.F. */
{
   const char * szP  = hb_parc( 2 );
   UNSIGNED32   ulRc = 1;
   if( szP )
      ulRc = AdsSetString( ( ADSHANDLE ) hb_parnint( 1 ), ( UNSIGNED8 * ) szP,
                           ( UNSIGNED8 * ) hb_parc( 3 ), ( UNSIGNED32 ) hb_parclen( 3 ) );
   hb_retl( ulRc == 0 );
}

HB_FUNC( HBO_BINDNUM )       /* ( nStmt, cParam, nVal ) -> .T./.F. */
{
   const char * szP  = hb_parc( 2 );
   UNSIGNED32   ulRc = 1;
   if( szP )
      ulRc = AdsSetDouble( ( ADSHANDLE ) hb_parnint( 1 ), ( UNSIGNED8 * ) szP, hb_parnd( 3 ) );
   hb_retl( ulRc == 0 );
}

HB_FUNC( HBO_BINDLOG )       /* ( nStmt, cParam, lVal ) -> .T./.F. */
{
   const char * szP  = hb_parc( 2 );
   UNSIGNED32   ulRc = 1;
   if( szP )
      ulRc = AdsSetLogical( ( ADSHANDLE ) hb_parnint( 1 ), ( UNSIGNED8 * ) szP,
                            ( UNSIGNED16 ) ( hb_parl( 3 ) ? 1 : 0 ) );
   hb_retl( ulRc == 0 );
}

HB_FUNC( HBO_BINDNULL )      /* ( nStmt, cParam ) -> .T./.F. */
{
   const char * szP  = hb_parc( 2 );
   UNSIGNED32   ulRc = 1;
   if( szP )
      ulRc = AdsSetEmpty( ( ADSHANDLE ) hb_parnint( 1 ), ( UNSIGNED8 * ) szP );
   hb_retl( ulRc == 0 );
}

HB_FUNC( HBO_EXECPREP )      /* ( nStmt, @nCursor ) -> .T./.F. */
{
   ADSHANDLE  hCur = 0;
   UNSIGNED32 ulRc = AdsExecuteSQL( ( ADSHANDLE ) hb_parnint( 1 ), &hCur );
   hb_stornint( ( HB_MAXINT ) hCur, 2 );
   hb_retl( ulRc == 0 );
}

HB_FUNC( HBO_FIELDTYPE )     /* ( nCur, cName ) -> 1-char tag: N/L/D/T/C */
{
   UNSIGNED16   usType  = 0;
   const char * szField = hb_parc( 2 );
   char         cTag    = 'C';
   UNSIGNED32   ulRc    = 1;
   if( szField )
   {
      ulRc = AdsGetFieldType( ( ADSHANDLE ) hb_parnint( 1 ),
                              ( UNSIGNED8 * ) szField, &usType );
      /* ACE/DBF backends may store field names in a different case than
       * the caller supplies.  On lookup failure try a lowercase copy so
       * tables created via SQL DDL (which may preserve original case) are
       * handled the same way as tables where names are stored in upper
       * case by convention. */
      if( ulRc != 0 )
      {
         char   szLo[ 256 ];
         size_t i, n = strlen( szField );
         if( n < sizeof( szLo ) )
         {
            for( i = 0; i < n; i++ )
               szLo[ i ] = ( char ) tolower( ( unsigned char ) szField[ i ] );
            szLo[ n ] = '\0';
            usType = 0;
            ulRc = AdsGetFieldType( ( ADSHANDLE ) hb_parnint( 1 ),
                                    ( UNSIGNED8 * ) szLo, &usType );
         }
      }
      if( ulRc == 0 )
      {
         switch( usType )
         {
            case ADS_NUMERIC:   cTag = 'N'; break;
#ifdef ADS_DOUBLE
            case ADS_DOUBLE:    cTag = 'N'; break;
#endif
#ifdef ADS_INTEGER
            case ADS_INTEGER:   cTag = 'N'; break;
#endif
#ifdef ADS_AUTOINC
            case ADS_AUTOINC:   cTag = 'N'; break;
#endif
            case ADS_LOGICAL:   cTag = 'L'; break;
            case ADS_DATE:      cTag = 'D'; break;
#ifdef ADS_TIMESTAMP
            case ADS_TIMESTAMP: cTag = 'T'; break;
#endif
#ifdef ADS_TIME
            case ADS_TIME:      cTag = 'T'; break;
#endif
            default:            cTag = 'C'; break;
         }
      }
   }
   hb_retclen( &cTag, 1 );
}

/* AdsGetDouble / AdsIsNull are inert on SQL result-set cursors over the
 * sqlite passthrough (they always return 0 / false on that path); they only
 * populate on navigational / native-table cursors.  Kept as seed for the
 * navigational backends.  SQL-cursor hydration reads the string via HBO_FIELD
 * and converts by field-type tag in Harbour (see Task 3). */
HB_FUNC( HBO_GETNUM )        /* ( nCur, cName ) -> double */
{
   double       dVal    = 0;
   const char * szField = hb_parc( 2 );
   if( szField )
      AdsGetDouble( ( ADSHANDLE ) hb_parnint( 1 ),
                    ( UNSIGNED8 * ) szField, &dVal );
   hb_retnd( dVal );
}

HB_FUNC( HBO_ISNULL )        /* ( nCur, cName ) -> .T. if field is NULL */
{
   UNSIGNED16   usNull  = 0;
   const char * szField = hb_parc( 2 );
   if( szField )
      AdsIsNull( ( ADSHANDLE ) hb_parnint( 1 ),
                 ( UNSIGNED8 * ) szField, &usNull );
   hb_retl( usNull != 0 );
}

HB_FUNC( HBO_FIELDDECIMALS )   /* ( nTable, cName ) -> decimal count (0 if none) */
{
   UNSIGNED16   usDec   = 0;
   const char * szField = hb_parc( 2 );
   if( szField )
   {
      UNSIGNED32 ulRc = AdsGetFieldDecimals( ( ADSHANDLE ) hb_parnint( 1 ),
                                             ( UNSIGNED8 * ) szField, &usDec );
      /* ACE/DBF backends may store field names in a different case than
       * the caller supplies.  On lookup failure try a lowercase copy so
       * tables created via SQL DDL (which may preserve original case) are
       * handled the same way as tables where names are stored in upper
       * case by convention. */
      if( ulRc != 0 )
      {
         char   szLo[ 256 ];
         size_t i, n = strlen( szField );
         if( n < sizeof( szLo ) )
         {
            for( i = 0; i < n; i++ )
               szLo[ i ] = ( char ) tolower( ( unsigned char ) szField[ i ] );
            szLo[ n ] = '\0';
            usDec = 0;
            AdsGetFieldDecimals( ( ADSHANDLE ) hb_parnint( 1 ),
                                 ( UNSIGNED8 * ) szLo, &usDec );
         }
      }
   }
   hb_retni( ( int ) usDec );
}

/* ---- navigational DDL + index seek (Fatia 9) -------------------------------
 * AdsCreateTable builds a DBF/CDX table from an xBase field-definition string
 * ("NAME,TYPE,WIDTH,DEC;..."). Indexes are CDX tags in the production bag
 * (pucFile NULL). Seek needs an index HANDLE (AdsGetIndexHandle) then AdsSeek,
 * which positions the table cursor; AdsIsFound reports the hit. */

HB_FUNC( HBO_CREATETABLE )   /* ( nConn, cName, cFields ) -> table handle, 0 fail */
{
   ADSHANDLE    hTable = 0;
   const char * szName = hb_parc( 2 );
   const char * szDef  = hb_parc( 3 );
   UNSIGNED32   ulRc   = 1;
   if( szName && szDef )
      ulRc = AdsCreateTable( ( ADSHANDLE ) hb_parnint( 1 ),
                             ( UNSIGNED8 * ) szName, NULL,
                             ADS_CDX, ADS_ANSI, ADS_PROPRIETARY_LOCKING,
                             ADS_IGNORERIGHTS, 0,
                             ( UNSIGNED8 * ) szDef, &hTable );
   hb_retnint( ulRc == 0 ? ( HB_MAXINT ) hTable : 0 );
}

HB_FUNC( HBO_CREATEINDEX )   /* ( nTbl, cTag, cExpr ) -> .T./.F. */
{
   /* AdsCreateIndex61 with an empty bag name derives the production CDX path
    * from the table file path (<stem>.cdx in the same directory), which is the
    * correct convention for a structural compound index. */
   ADSHANDLE    hIndex   = 0;
   const char * szTag    = hb_parc( 2 );
   const char * szExpr   = hb_parc( 3 );
   UNSIGNED8    szEmpty[] = { 0 };
   UNSIGNED32   ulRc     = 1;
   if( szTag && szExpr )
      ulRc = AdsCreateIndex61( ( ADSHANDLE ) hb_parnint( 1 ),
                               szEmpty, ( UNSIGNED8 * ) szTag,
                               ( UNSIGNED8 * ) szExpr, NULL, NULL, 0, 0, &hIndex );
   hb_retl( ulRc == 0 );
}

/* Open the table's structural index bag (the <stem>.cdx beside the data file)
 * so every tag in it becomes navigable on this cursor.  The engine resolves a
 * relative bag against the table's own directory, so the bag MUST be passed as
 * a BARE name (stem + ".cdx") -- a full path would be re-joined to the dir and
 * miss (and the auto-open inside AdsOpenTable, which passes a full path, leaves
 * the index unbound on this build).  Idempotent.  Returns bound index count. */
static int oa_open_struct_bag( ADSHANDLE hTable )
{
   char       szPath[ 1024 ];
   UNSIGNED16 usPLen = ( UNSIGNED16 ) ( sizeof( szPath ) - 5 );
   char *     pBase;
   char *     pDot;
   char *     pSep1;
   char *     pSep2;
   char *     pSep;
   ADSHANDLE  aHandles[ 64 ];
   UNSIGNED16 usLen = ( UNSIGNED16 ) ( sizeof( aHandles ) / sizeof( aHandles[ 0 ] ) );
   if( AdsGetTableFilename( hTable, ADS_FULLPATHNAME,
                            ( UNSIGNED8 * ) szPath, &usPLen ) != 0 || usPLen == 0 )
      return 0;
   pSep1 = strrchr( szPath, '\\' );
   pSep2 = strrchr( szPath, '/' );
   pSep  = pSep1 > pSep2 ? pSep1 : pSep2;
   pBase = ( pSep != NULL ) ? pSep + 1 : szPath;
   pDot  = strrchr( pBase, '.' );
   if( pDot != NULL )
      *pDot = '\0';
   strcat( pBase, ".cdx" );
   if( AdsOpenIndex( hTable, ( UNSIGNED8 * ) pBase, aHandles, &usLen ) != 0 )
      return 0;
   return ( int ) usLen;
}

HB_FUNC( HBO_OPENIDXBAG )    /* ( nTbl ) -> bound index count (0 if none/fail) */
{
   hb_retni( oa_open_struct_bag( ( ADSHANDLE ) hb_parnint( 1 ) ) );
}

HB_FUNC( HBO_GETINDEX )      /* ( nTbl, cTag ) -> index handle, 0 if none */
{
   ADSHANDLE    hTable = ( ADSHANDLE ) hb_parnint( 1 );
   ADSHANDLE    hIndex = 0;
   const char * szTag  = hb_parc( 2 );
   UNSIGNED32   ulRc   = 1;
   if( szTag )
   {
      ulRc = AdsGetIndexHandle( hTable, ( UNSIGNED8 * ) szTag, &hIndex );
      /* Tag not bound yet -> open the structural bag and retry. */
      if( ulRc != 0 )
      {
         oa_open_struct_bag( hTable );
         hIndex = 0;
         ulRc = AdsGetIndexHandle( hTable, ( UNSIGNED8 * ) szTag, &hIndex );
      }
   }
   hb_retnint( ulRc == 0 ? ( HB_MAXINT ) hIndex : 0 );
}

HB_FUNC( HBO_SEEKSTR )       /* ( nIdx, cKey ) -> .T. if found (soft seek) */
{
   const char * szKey  = hb_parc( 2 );
   UNSIGNED16   bFound = 0;
   UNSIGNED32   ulRc   = 1;
   if( szKey )
      ulRc = AdsSeek( ( ADSHANDLE ) hb_parnint( 1 ),
                      ( UNSIGNED8 * ) szKey, ( UNSIGNED16 ) hb_parclen( 2 ),
                      ADS_STRINGKEY, ADS_SOFTSEEK, &bFound );
   hb_retl( ulRc == 0 && bFound != 0 );
}

HB_FUNC( HBO_SEEKNUM )       /* ( nIdx, nVal ) -> .T. if found (soft seek, double key) */
{
   double     dKey   = hb_parnd( 2 );
   UNSIGNED16 bFound = 0;
   UNSIGNED32 ulRc   = AdsSeek( ( ADSHANDLE ) hb_parnint( 1 ),
                                ( UNSIGNED8 * ) &dKey, ( UNSIGNED16 ) sizeof( dKey ),
                                ADS_DOUBLEKEY, ADS_SOFTSEEK, &bFound );
   hb_retl( ulRc == 0 && bFound != 0 );
}

HB_FUNC( HBO_SETSCOPENUM )   /* ( nIdx, nScope, nVal ) -> .T./.F. (rc==0) */
{
   /* ADS_TOP=0, ADS_BOTTOM=1; ADS_DOUBLEKEY=2 */
   double dv = hb_parnd( 3 );
   UNSIGNED32 ulRc = AdsSetScope( ( ADSHANDLE ) hb_parnint( 1 ),
                                  ( UNSIGNED16 ) hb_parni( 2 ),
                                  ( UNSIGNED8 * ) &dv,
                                  ( UNSIGNED16 ) sizeof( double ),
                                  ADS_DOUBLEKEY );
   hb_retl( ulRc == 0 );
}

HB_FUNC( HBO_SETSCOPESTR )   /* ( nIdx, nScope, cVal ) -> .T./.F. (rc==0) */
{
   /* ADS_STRINGKEY=1 */
   const char * sz = hb_parc( 3 );
   UNSIGNED32   ulRc = 1;
   if( sz )
      ulRc = AdsSetScope( ( ADSHANDLE ) hb_parnint( 1 ),
                          ( UNSIGNED16 ) hb_parni( 2 ),
                          ( UNSIGNED8 * ) sz,
                          ( UNSIGNED16 ) hb_parclen( 3 ),
                          ADS_STRINGKEY );
   hb_retl( ulRc == 0 );
}

HB_FUNC( HBO_CLEARSCOPE )    /* ( nIdx, nScope ) -> .T./.F. (rc==0) */
{
   hb_retl( AdsClearScope( ( ADSHANDLE ) hb_parnint( 1 ),
                           ( UNSIGNED16 ) hb_parni( 2 ) ) == 0 );
}

HB_FUNC( HBO_ISFOUND )       /* ( nTbl ) -> .T./.F. */
{
   UNSIGNED16 bFound = 0;
   AdsIsFound( ( ADSHANDLE ) hb_parnint( 1 ), &bFound );
   hb_retl( bFound != 0 );
}

/* ---- Data Dictionary (Frente 2: o hibrido) --------------------------------
 * Le metadados ricos por campo do dicionario vivo (REQUIRED/DEFAULT). O handle
 * e o da CONEXAO (oConn:Handle()), nao o de uma tabela. Sem dicionario ou sem
 * a propriedade -> string vazia (fallback gracioso na camada Harbour). */
HB_FUNC( HBO_DDGETFIELDPROP )  /* ( nConn, cTable, cField, nProp ) -> cValor ("" se nenhum) */
{
   char       szBuf[ 512 ];
   UNSIGNED16 usLen = ( UNSIGNED16 ) sizeof( szBuf );
   memset( szBuf, 0, sizeof( szBuf ) );
   if( AdsDDGetFieldProperty( ( ADSHANDLE ) hb_parnint( 1 ),
          ( UNSIGNED8 * ) hb_parc( 2 ), ( UNSIGNED8 * ) hb_parc( 3 ),
          ( UNSIGNED16 ) hb_parni( 4 ), szBuf, &usLen ) == 0 )
   {
      if( usLen > ( UNSIGNED16 ) ( sizeof( szBuf ) - 1 ) )
         usLen = ( UNSIGNED16 ) ( sizeof( szBuf ) - 1 );
      szBuf[ usLen ] = '\0';
      hb_retc( szBuf );
   }
   else
      hb_retc( "" );
}

/* --- helpers de FIXTURE de dicionario (usados pelo gate p/ montar um .add) --- */
HB_FUNC( HBO_DDCREATE )        /* ( cDictPath ) -> nConn (0 em falha) */
{
   ADSHANDLE  hConn = 0;
   UNSIGNED32 ulRc  = AdsDDCreate( ( UNSIGNED8 * ) hb_parc( 1 ), 0, NULL, &hConn );
   hb_retnint( ulRc == 0 ? ( HB_MAXINT ) hConn : 0 );
}

HB_FUNC( HBO_DDADDTABLE )      /* ( nConn, cAlias, cTablePath ) -> .T./.F. */
{
   hb_retl( AdsDDAddTable( ( ADSHANDLE ) hb_parnint( 1 ),
      ( UNSIGNED8 * ) hb_parc( 2 ), ( UNSIGNED8 * ) hb_parc( 3 ),
      0, 0, NULL, NULL ) == 0 );
}

HB_FUNC( HBO_DDSETFIELDPROP )  /* ( nConn, cTable, cField, nProp, cValor ) -> .T./.F. */
{
   hb_retl( AdsDDSetFieldProperty( ( ADSHANDLE ) hb_parnint( 1 ),
      ( UNSIGNED8 * ) hb_parc( 2 ), ( UNSIGNED8 * ) hb_parc( 3 ),
      ( UNSIGNED16 ) hb_parni( 4 ),
      ( void * ) hb_parc( 5 ), ( UNSIGNED16 ) hb_parclen( 5 ) ) == 0 );
}

/* ---- AOF (Rushmore Optimized Filter) server-side pushdown (Task 1) -------- *
 * AdsSetAOF instala um filtro avaliado no servidor (AOF); AdsClearAOF remove.
 * AdsGetAOFOptLevel devolve o nivel de otimizacao Rushmore (0 = sem indice
 * cobrindo o filtro, ADS_OPTIMIZED_NONE; apenas diagnostico, nunca asserir
 * != 0, pois tabela sem indice SALDO retorna 0 mesmo com filtro ativo).     */

HB_FUNC( HBO_SETAOF )       /* ( nTbl, cCond ) -> .T./.F. (rc==0) */
{
   const char * szCond = hb_parc( 2 );
   UNSIGNED32   ulRc   = 1;
   if( szCond )
      ulRc = AdsSetAOF( ( ADSHANDLE ) hb_parnint( 1 ),
                        ( UNSIGNED8 * ) szCond, ADS_RESOLVE_IMMEDIATE );
   hb_retl( ulRc == 0 );
}

HB_FUNC( HBO_CLEARAOF )     /* ( nTbl ) -> .T./.F. (rc==0) */
{
   hb_retl( AdsClearAOF( ( ADSHANDLE ) hb_parnint( 1 ) ) == 0 );
}

HB_FUNC( HBO_AOFOPTLEVEL )  /* ( nTbl ) -> nLevel (0 == ADS_OPTIMIZED_NONE); diagnostico */
{
   UNSIGNED16 usLevel = 0;
   /* assinatura ACE real = 4 args: (hTable, *pusLevel, *pucBuf, *pusLen) */
   AdsGetAOFOptLevel( ( ADSHANDLE ) hb_parnint( 1 ), &usLevel, NULL, NULL );
   hb_retni( ( int ) usLevel );
}

#pragma ENDDUMP
