---
title: AdsStmtSetTableLockType
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-stmt-set-table-lock-type/
---

# AdsStmtSetTableLockType

Define o tipo de bloqueio da tabela para o statement.

## Sintaxe

```c
UNSIGNED32 AdsStmtSetTableLockType(ADSHANDLE h, UNSIGNED16 us);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `h` | `ADSHANDLE` | Handle do statement. |
| `us` | `UNSIGNED16` | Tipo de bloqueio. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsStmtSetTableLockType` define o tipo de bloqueio da tabela para operações SQL.

## Exemplo

```c
AdsStmtSetTableLockType(hStmt, ADS_PROPRIETARY_LOCKING);
```

## Ver Também

- [AdsStmtSetTablePassword]({{ site.baseurl }}/pt/funcoes/ads-stmt-set-table-password/)
- [AdsStmtSetTableType]({{ site.baseurl }}/pt/funcoes/ads-stmt-set-table-type/)
- [AdsExecuteSQLDirect]({{ site.baseurl }}/pt/funcoes/ads-execute-sql-direct/)

---

[AdsStmtSetTablePassword →]({{ site.baseurl }}/pt/funcoes/ads-stmt-set-table-password/)
