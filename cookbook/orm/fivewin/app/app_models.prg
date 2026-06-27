/* app_models.prg -- models do app de exemplo. Dominio GENERICO (clientes 1:N
   pedidos), sem nenhuma referencia a dominio real. Linka SO o ORM (sem FiveWin). */
#include "hborm.ch"

CREATE CLASS TCliente FROM TORMModel
   METHOD TableName()   INLINE "clientes"
   METHOD SoftDeletes() INLINE .T.
   METHOD Casts()       INLINE { "limite" => "decimal:2" }
   METHOD Relations()   INLINE { ;
      "pedidos" => ORM_HasMany( {| c | TPedido():New( c ) }, "cliente_id" ) }
END CLASS

CREATE CLASS TPedido FROM TORMModel
   METHOD TableName()   INLINE "pedidos"
   METHOD SoftDeletes() INLINE .T.
   METHOD Casts()       INLINE { "valor" => "decimal:2", "cliente_id" => "integer", "data" => "date" }
   METHOD Relations()   INLINE { ;
      "cliente" => ORM_BelongsTo( {| c | TCliente():New( c ) }, "cliente_id" ) }
END CLASS
