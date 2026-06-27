---
title: AdsExecuteSQL
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-execute-sql/
---

# AdsExecuteSQL

Executa uma consulta SQL preparada.

## Sintaxe

```c
UNSIGNED32 AdsExecuteSQL(ADSHANDLE hStatement, ADSHANDLE* phCursor);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hStatement` | `ADSHANDLE` | Handle do statement preparado. |
| `phCursor` | `ADSHANDLE*` | Ponteiro para receber o handle do cursor. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_PARSE_ERROR` se não houver SQL preparado.

## Descrição

`AdsExecuteSQL` executa uma consulta SQL previamente preparada com `AdsPrepareSQL` e devolve um cursor.

## Exemplo

```c
AdsPrepareSQL(hStmt, "SELECT * FROM clientes WHERE id = :id");
AdsSetString(hStmt, "id", "100", 3);
ADSHANDLE hCursor;
AdsExecuteSQL(hStmt, &hCursor);
```

## Ver Também

- [AdsPrepareSQL]({{ site.baseurl }}/pt/funcoes/ads-prepare-sql/)
- [AdsExecuteSQLDirect]({{ site.baseurl }}/pt/funcoes/ads-execute-sql-direct/)
- [AdsGetNumParams]({{ site.baseurl }}/pt/funcoes/ads-get-num-params/)

---

[AdsSetCollation →]({{ site.baseurl }}/pt/funcoes/ads-set-collation/)
