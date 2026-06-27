---
title: AdsPrepareSQL
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-prepare-sql/
---

# AdsPrepareSQL

Prepara uma consulta SQL.

## Sintaxe

```c
UNSIGNED32 AdsPrepareSQL(ADSHANDLE hStatement, UNSIGNED8* pucSQL);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hStatement` | `ADSHANDLE` | Handle do statement. |
| `pucSQL` | `UNSIGNED8*` | Consulta SQL a preparar. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsPrepareSQL` prepara uma consulta SQL para execução posterior.

## Exemplo

```c
AdsPrepareSQL(hStmt, "SELECT * FROM clientes WHERE id = :id");
```

## Ver Também

- [AdsExecuteSQLDirect]({{ site.baseurl }}/pt/funcoes/ads-execute-sql-direct/)
- [AdsGetNumParams]({{ site.baseurl }}/pt/funcoes/ads-get-num-params/)
- [AdsCloseSQLStatement]({{ site.baseurl }}/pt/funcoes/ads-close-sql-statement/)

---

[AdsClearSQLParams →]({{ site.baseurl }}/pt/funcoes/ads-clear-sql-params/)
