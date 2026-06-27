/* app_backends.prg -- abertura de conexao por tipo + seed idempotente. SQLite e
   DBF rodam; ADT e MariaDB degradam com motivo (preenchidos em tasks futuras). */
#include "hborm.ch"

#define DBF_DIR   "navdata_app"

FUNCTION AbrirBackend( cTipo )
   LOCAL hR := { "ok" => .F., "conn" => NIL, "motivo" => "", "tipo" => cTipo }
   LOCAL oConn, cUri
   DO CASE
   CASE cTipo == "sqlite"
      hbo_SetTableType( 0 )
      cUri := "sqlite3://" + DataDir() + "orm_app.db"
   CASE cTipo == "dbf"
      hbo_SetTableType( 0 )
      cUri := "dbf://" + DataDir() + DBF_DIR
      hb_DirCreate( DataDir() + DBF_DIR )
   CASE cTipo == "adt"
      hb_DirCreate( DataDir() + DBF_DIR )
      BEGIN SEQUENCE WITH {| e | Break( e ) }
         hbo_SetTableType( 3 )                  // ADS_ADT
         cUri  := "dbf://" + DataDir() + DBF_DIR
         oConn := ORMConnect( cUri )
         IF oConn == NIL .OR. ! oConn:IsOpen()
            Break( "conexao ADT falhou" )
         ENDIF
         /* sonda leve: cria tabela-probe ADT e verifica o arquivo .adt gerado */
         TORMSchema():New( oConn ):CreateTable( "adt_probe", {| t | t:Id(), t:String( "x", 4 ) } )
         IF ! File( DataDir() + DBF_DIR + "\adt_probe.adt" )
            Break( "ADT nao gerou .adt" )
         ENDIF
         hR[ "ok" ]   := .T.
         hR[ "conn" ] := oConn
      RECOVER
         hbo_SetTableType( 0 )
         IF oConn != NIL ; oConn:Close() ; ENDIF
         hR[ "motivo" ] := "engine sem suporte ADT neste build (degradado)"
      END SEQUENCE
      RETURN hR
   CASE cTipo == "maria"
      hbo_SetTableType( 0 )                      // reset tipo (contrato: cada ramo ajusta no inicio)
      cUri := GetEnv( "HBORM_MARIA_URI" )
      IF Empty( cUri )
         hR[ "motivo" ] := "defina HBORM_MARIA_URI e tenha um servidor MariaDB no ar"
         RETURN hR
      ENDIF
      BEGIN SEQUENCE WITH {| e | Break( e ) }
         oConn := ORMConnect( cUri )
         IF oConn == NIL .OR. ! oConn:IsOpen()
            Break( "sem conexao" )
         ENDIF
         hR[ "ok" ]   := .T.
         hR[ "conn" ] := oConn
      RECOVER
         IF oConn != NIL ; oConn:Close() ; ENDIF
         hR[ "ok" ]     := .F.
         hR[ "conn" ]   := NIL
         hR[ "motivo" ] := "MariaDB indisponivel (servidor fora do ar ou backend ausente neste build)"
      END SEQUENCE
      RETURN hR
   OTHERWISE
      hR[ "motivo" ] := "tipo desconhecido: " + hb_CStr( cTipo )
      RETURN hR
   ENDCASE
   oConn := ORMConnect( cUri )
   IF oConn == NIL .OR. ! oConn:IsOpen()
      hR[ "motivo" ] := "nao consegui abrir: " + cTipo
      RETURN hR
   ENDIF
   hR[ "ok" ]   := .T.
   hR[ "conn" ] := oConn
   RETURN hR

STATIC FUNCTION DataDir()
   RETURN hb_DirBase()

FUNCTION SeedDados( oConn )
   DropTabelas( oConn )
   TORMSchema():New( oConn ):CreateTable( "clientes", {| t | ;
      t:Id(), ;
      t:String( "nome", 40 ):Nullable( .F. ), ;
      t:String( "cidade", 30 ), ;
      t:Decimal( "limite", 12, 2 ), ;
      t:DateTime( "deleted_at" ):Nullable( .T. ) } )
   TORMSchema():New( oConn ):CreateTable( "pedidos", {| t | ;
      t:Id(), ;
      t:Integer( "cliente_id" ), ;
      t:String( "descricao", 60 ), ;
      t:Decimal( "valor", 12, 2 ), ;
      t:Date( "data" ), ;
      t:DateTime( "deleted_at" ):Nullable( .T. ) } )
   SeedClientes( oConn )
   SeedPedidos( oConn )
   RETURN NIL

FUNCTION DropTabelas( oConn )
   IF oConn:IsNavigational()
      FErase( DataDir() + DBF_DIR + "\pedidos.dbf" )
      FErase( DataDir() + DBF_DIR + "\pedidos.cdx" )
      FErase( DataDir() + DBF_DIR + "\pedidos.adt" )
      FErase( DataDir() + DBF_DIR + "\pedidos.adi" )
      FErase( DataDir() + DBF_DIR + "\clientes.dbf" )
      FErase( DataDir() + DBF_DIR + "\clientes.cdx" )
      FErase( DataDir() + DBF_DIR + "\clientes.adt" )
      FErase( DataDir() + DBF_DIR + "\clientes.adi" )
      FErase( DataDir() + DBF_DIR + "\adt_probe.adt" )
      FErase( DataDir() + DBF_DIR + "\adt_probe.adi" )
   ELSE
      oConn:Execute( "DROP TABLE IF EXISTS pedidos",  {} )
      oConn:Execute( "DROP TABLE IF EXISTS clientes", {} )
   ENDIF
   RETURN NIL

STATIC PROCEDURE SeedClientes( oConn )
   LOCAL aData := { ;
      { "Maria Silva",     "Sao Paulo",      5000.00 }, ;
      { "John Doe",        "Rio de Janeiro", 3200.50 }, ;
      { "Ana Pereira",     "Belo Horizonte", 8750.00 }, ;
      { "Carlos Souza",    "Curitiba",       1500.00 }, ;
      { "Beatriz Lima",    "Porto Alegre",   9900.90 }, ;
      { "Diego Fernandez", "Salvador",       4100.00 } }, a, n := 0
   FOR EACH a IN aData
      n++
      TCliente():New( oConn ):Create( { "id" => n, ;
         "nome" => a[ 1 ], "cidade" => a[ 2 ], "limite" => a[ 3 ] } )
   NEXT
   RETURN

STATIC PROCEDURE SeedPedidos( oConn )
   LOCAL aData := { ;
      { 1, "Pedido inicial",  250.00, 0d20260105 }, ;
      { 1, "Recompra",        120.50, 0d20260210 }, ;
      { 2, "Assinatura",      900.00, 0d20260112 }, ;
      { 3, "Projeto grande", 4200.00, 0d20260118 }, ;
      { 3, "Ajuste",          150.00, 0d20260220 }, ;
      { 4, "Compra unica",    300.00, 0d20260201 } }, a, n := 0
   FOR EACH a IN aData
      n++
      TPedido():New( oConn ):Create( { "id" => n, ;
         "cliente_id" => a[ 1 ], "descricao" => a[ 2 ], "valor" => a[ 3 ], "data" => a[ 4 ] } )
   NEXT
   RETURN
