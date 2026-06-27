---
title: AdsStmtSetTableType
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-stmt-set-table-type/
---

# AdsStmtSetTableType

Define o tipo da tabela para o statement.

## Sintaxe

```c
UNSIGNED32 AdsStmtSetTableType(ADSHANDLE h, UNSIGNED16 us);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `h` | `ADSHANDLE` | Handle do statement. |
| `us` | `UNSIGNED16` | Tipo da tabela (ADS_CDX, ADS_NTX, ADS_ADT). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsStmtSetTableType` define o tipo da tabela para operações SQL.

## Exemplo

```c
AdsStmtSetTableType(hStmt, ADS_CDX);
```

## Ver Também

- [AdsStmtSetTableLockType]({{ site.baseurl }}/pt/funcoes/ads-stmt-set-table-lock-type/)
- [AdsStmtSetTableReadOnly]({{ site.baseurl }}/pt/funcoes/ads-stmt-set-table-read-only/)
- [AdsExecuteSQLDirect]({{ site.baseurl }}/pt/funcoes/ads-execute-sql-direct/)

---

[AdsTestLogin →]({{ site.baseurl }}/pt/funcoes/ads-test-login/)
