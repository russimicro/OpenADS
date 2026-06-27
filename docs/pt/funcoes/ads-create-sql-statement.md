---
title: AdsCreateSQLStatement
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-create-sql-statement/
---

# AdsCreateSQLStatement

Cria um statement SQL.

## Sintaxe

```c
UNSIGNED32 AdsCreateSQLStatement(ADSHANDLE hConnect, ADSHANDLE* phStatement);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `phStatement` | `ADSHANDLE*` | Ponteiro para receber o handle do statement. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INVALID_CONNECTION_HANDLE` se a conexão for inválida.

## Descrição

`AdsCreateSQLStatement` cria um novo statement SQL para execução de consultas.

## Exemplo

```c
ADSHANDLE hStmt;
AdsCreateSQLStatement(hConnect, &hStmt);
```

## Ver Também

- [AdsExecuteSQLDirect]({{ site.baseurl }}/pt/funcoes/ads-execute-sql-direct/)
- [AdsCloseSQLStatement]({{ site.baseurl }}/pt/funcoes/ads-close-sql-statement/)
- [AdsVerifySQL]({{ site.baseurl }}/pt/funcoes/ads-verify-sql/)

---

[AdsExecuteSQLDirect →]({{ site.baseurl }}/pt/funcoes/ads-execute-sql-direct/)
