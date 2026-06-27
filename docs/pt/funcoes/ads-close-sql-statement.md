---
title: AdsCloseSQLStatement
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-close-sql-statement/
---

# AdsCloseSQLStatement

Fecha um statement SQL.

## Sintaxe

```c
UNSIGNED32 AdsCloseSQLStatement(ADSHANDLE hStatement);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hStatement` | `ADSHANDLE` | Handle do statement. |

## Valor de Retorno

`AE_SUCCESS` (0) sempre.

## Descrição

`AdsCloseSQLStatement` fecha um statement SQL e liberta os recursos associados.

## Exemplo

```c
AdsCloseSQLStatement(hStmt);
```

## Ver Também

- [AdsCreateSQLStatement]({{ site.baseurl }}/pt/funcoes/ads-create-sql-statement/)
- [AdsExecuteSQLDirect]({{ site.baseurl }}/pt/funcoes/ads-execute-sql-direct/)
- [AdsPrepareSQL]({{ site.baseurl }}/pt/funcoes/ads-prepare-sql/)

---

[AdsPrepareSQL →]({{ site.baseurl }}/pt/funcoes/ads-prepare-sql/)
