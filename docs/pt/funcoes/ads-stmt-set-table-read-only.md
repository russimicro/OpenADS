---
title: AdsStmtSetTableReadOnly
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-stmt-set-table-read-only/
---

# AdsStmtSetTableReadOnly

Define se a tabela é apenas leitura para o statement.

## Sintaxe

```c
UNSIGNED32 AdsStmtSetTableReadOnly(ADSHANDLE h, UNSIGNED16 us);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `h` | `ADSHANDLE` | Handle do statement. |
| `us` | `UNSIGNED16` | 1 para apenas leitura, 0 para leitura/escrita. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsStmtSetTableReadOnly` define se a tabela é apenas leitura para operações SQL.

## Exemplo

```c
AdsStmtSetTableReadOnly(hStmt, 1);
```

## Ver Também

- [AdsStmtSetTableLockType]({{ site.baseurl }}/pt/funcoes/ads-stmt-set-table-lock-type/)
- [AdsStmtSetTableType]({{ site.baseurl }}/pt/funcoes/ads-stmt-set-table-type/)
- [AdsExecuteSQLDirect]({{ site.baseurl }}/pt/funcoes/ads-execute-sql-direct/)

---

[AdsStmtSetTableType →]({{ site.baseurl }}/pt/funcoes/ads-stmt-set-table-type/)
