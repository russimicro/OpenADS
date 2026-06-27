/*
 * OpenADS + PostgreSQL example (OpenADS Plus backend).
 *
 * Drives a PostgreSQL table through the ACE API exactly like a local DBF:
 * connect with a `postgresql://` URI, AdsOpenTable, then the usual xBase
 * navigation and field getters, plus an Append/Replace/Write insert.
 *
 * The engine must be built with -DOPENADS_WITH_POSTGRESQL=ON (libpq).
 * Without it, AdsConnect on a postgresql:// URI returns
 * AE_FUNCTION_NOT_AVAILABLE (5004).
 *
 * Expected schema (see README.md):
 *   CREATE TABLE clientes (id INTEGER PRIMARY KEY, nome TEXT, saldo DOUBLE PRECISION);
 */
#include "openads/ace.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Print the last ACE error and return it, for terse failure handling. */
static UNSIGNED32 die(const char* where, UNSIGNED32 rc) {
    UNSIGNED8  buf[256] = {0};
    UNSIGNED16 len = (UNSIGNED16)sizeof(buf);
    UNSIGNED32 code = rc;
    AdsGetLastError(&code, buf, &len);
    fprintf(stderr, "%s failed: rc=%lu code=%lu %.*s\n",
            where, (unsigned long)rc, (unsigned long)code, (int)len, buf);
    return rc;
}

int main(int argc, char** argv) {
    /* URI from argv[1], else $OPENADS_PG_URI, else a sane local default. */
    const char* uri = (argc > 1) ? argv[1] : getenv("OPENADS_PG_URI");
    if (uri == NULL || uri[0] == '\0')
        uri = "postgresql://postgres@127.0.0.1:5432/postgres";

    ADSHANDLE hConn = 0, hTable = 0;
    UNSIGNED32 rc;

    rc = AdsConnect60((UNSIGNED8*)uri, ADS_REMOTE_SERVER,
                      NULL, NULL, 0, &hConn);
    if (rc != 0) return (int)die("AdsConnect60", rc);
    printf("connected: %s\n", uri);

    UNSIGNED8 name[]  = "clientes";
    UNSIGNED8 alias[] = "clientes";
    rc = AdsOpenTable(hConn, name, alias, ADS_DEFAULT, 0, 0, 0,
                      ADS_DEFAULT, &hTable);
    if (rc != 0) { die("AdsOpenTable", rc); AdsDisconnect(hConn); return (int)rc; }

    UNSIGNED16 nfields = 0;
    AdsGetNumFields(hTable, &nfields);
    UNSIGNED32 count = 0;
    AdsGetRecordCount(hTable, 0, &count);
    printf("opened clientes: %u fields, %lu rows\n",
           nfields, (unsigned long)count);

    /* --- Read: classic xBase walk --- */
    puts("--- rows ---");
    AdsGotoTop(hTable);
    for (;;) {
        UNSIGNED16 eof = 0;
        AdsAtEOF(hTable, &eof);
        if (eof) break;

        UNSIGNED8  fNome[]  = "nome";
        UNSIGNED8  fSaldo[] = "saldo";
        UNSIGNED8  sbuf[128] = {0};
        UNSIGNED32 scap = (UNSIGNED32)sizeof(sbuf);
        double     saldo = 0.0;

        AdsGetString(hTable, fNome, sbuf, &scap, 0);
        AdsGetDouble(hTable, fSaldo, &saldo);
        printf("  nome=%-16.*s saldo=%.2f\n", (int)scap, sbuf, saldo);

        AdsSkip(hTable, 1);
    }

    /* --- Write: INSERT via Append + Replace + Write --- */
    {
        UNSIGNED8 fNome[]  = "nome";
        UNSIGNED8 fSaldo[] = "saldo";
        rc = AdsAppendRecord(hTable);
        if (rc == 0) {
            AdsSetString(hTable, fNome, (UNSIGNED8*)"Dani", 4);
            AdsSetDouble(hTable, fSaldo, 42.0);
            rc = AdsWriteRecord(hTable);
            if (rc == 0) puts("inserted: Dani / 42.00");
            else die("AdsWriteRecord", rc);
        } else {
            die("AdsAppendRecord", rc);
        }
    }

    AdsCloseTable(hTable);
    AdsDisconnect(hConn);
    puts("done.");
    return 0;
}
