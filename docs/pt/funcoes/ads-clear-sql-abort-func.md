---
title: AdsClearSQLAbortFunc
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-clear-sql-abort-func/
---

# AdsClearSQLAbortFunc

Remove a função de aborto SQL.

## Sintaxe

```c
UNSIGNED32 AdsClearSQLAbortFunc(void);
```

## Parâmetros

Nenhum.

## Valor de Retorno

`AE_SUCCESS` (0) sempre.

## Descrição

`AdsClearSQLAbortFunc` remove a função de callback de aborto SQL.

## Exemplo

```c
AdsClearSQLAbortFunc();
```

## Ver Também

- [AdsCreateSQLStatement]({{ site.baseurl }}/pt/funcoes/ads-create-sql-statement/)
- [AdsExecuteSQLDirect]({{ site.baseurl }}/pt/funcoes/ads-execute-sql-direct/)
- [AdsClearSQLParams]({{ site.baseurl }}/pt/funcoes/ads-clear-sql-params/)

---

[AdsStmtSetTableLockType →]({{ site.baseurl }}/pt/funcoes/ads-stmt-set-table-lock-type/)
