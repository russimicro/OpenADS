---
title: AdsGetNumParams
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-num-params/
---

# AdsGetNumParams

Retorna o número de parâmetros de um statement SQL.

## Sintaxe

```c
UNSIGNED32 AdsGetNumParams(ADSHANDLE hStatement, UNSIGNED16* pusNumParams);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hStatement` | `ADSHANDLE` | Handle do statement. |
| `pusNumParams` | `UNSIGNED16*` | Ponteiro para receber o número de parâmetros. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsGetNumParams` retorna o número de parâmetros nomeados (`:param`) no statement SQL.

## Exemplo

```c
UNSIGNED16 usParams;
AdsGetNumParams(hStmt, &usParams);
// usParams contém o número de parâmetros
```

## Ver Também

- [AdsCreateSQLStatement]({{ site.baseurl }}/pt/funcoes/ads-create-sql-statement/)
- [AdsSetString]({{ site.baseurl }}/pt/funcoes/ads-set-string/)
- [AdsExecuteSQLDirect]({{ site.baseurl }}/pt/funcoes/ads-execute-sql-direct/)

---

[AdsCreateSQLStatement →]({{ site.baseurl }}/pt/funcoes/ads-create-sql-statement/)
