---
title: AdsClearSQLParams
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-clear-sql-params/
---

# AdsClearSQLParams

Limpa os parâmetros de um statement SQL.

## Sintaxe

```c
UNSIGNED32 AdsClearSQLParams(ADSHANDLE hStatement);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hStatement` | `ADSHANDLE` | Handle do statement. |

## Valor de Retorno

`AE_SUCCESS` (0) sempre.

## Descrição

`AdsClearSQLParams` limpa todos os parâmetros definidos para um statement SQL.

## Exemplo

```c
AdsClearSQLParams(hStmt);
```

## Ver Também

- [AdsSetString]({{ site.baseurl }}/pt/funcoes/ads-set-string/)
- [AdsSetDouble]({{ site.baseurl }}/pt/funcoes/ads-set-double/)
- [AdsExecuteSQLDirect]({{ site.baseurl }}/pt/funcoes/ads-execute-sql-direct/)

---

[AdsClearSQLAbortFunc →]({{ site.baseurl }}/pt/funcoes/ads-clear-sql-abort-func/)
