---
title: AdsGetNumOpenTables
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-num-open-tables/
---

# AdsGetNumOpenTables

Retorna o número de tabelas abertas.

## Sintaxe

```c
UNSIGNED32 AdsGetNumOpenTables(UNSIGNED16* p);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `p` | `UNSIGNED16*` | Ponteiro para receber o número de tabelas abertas. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsGetNumOpenTables` retorna o número total de tabelas abertas, incluindo tabelas locais, remotas, SQLite, MSSQL, MariaDB, PostgreSQL, ODBC e Firebird.

## Exemplo

```c
UNSIGNED16 usCount;
AdsGetNumOpenTables(&usCount);
// usCount contém o número de tabelas abertas
```

## Ver Também

- [AdsGetAllTables]({{ site.baseurl }}/pt/funcoes/ads-get-all-tables/)
- [AdsOpenTable]({{ site.baseurl }}/pt/funcoes/ads-open-table/)
- [AdsCloseTable]({{ site.baseurl }}/pt/funcoes/ads-close-table/)

---

[AdsCreateTable →]({{ site.baseurl }}/pt/funcoes/ads-create-table/)
