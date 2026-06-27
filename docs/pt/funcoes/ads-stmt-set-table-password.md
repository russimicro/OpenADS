---
title: AdsStmtSetTablePassword
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-stmt-set-table-password/
---

# AdsStmtSetTablePassword

Define a palavra-passe de uma tabela para o statement.

## Sintaxe

```c
UNSIGNED32 AdsStmtSetTablePassword(ADSHANDLE h, UNSIGNED8* pTable,
                                   UNSIGNED8* pPwd);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `h` | `ADSHANDLE` | Handle do statement. |
| `pTable` | `UNSIGNED8*` | Nome da tabela. |
| `pPwd` | `UNSIGNED8*` | Palavra-passe. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsStmtSetTablePassword` define a palavra-passe de uma tabela para operações SQL.

## Exemplo

```c
AdsStmtSetTablePassword(hStmt, "clientes", "senha123");
```

## Ver Também

- [AdsStmtClearTablePasswords]({{ site.baseurl }}/pt/funcoes/ads-stmt-clear-table-passwords/)
- [AdsStmtSetTableLockType]({{ site.baseurl }}/pt/funcoes/ads-stmt-set-table-lock-type/)
- [AdsExecuteSQLDirect]({{ site.baseurl }}/pt/funcoes/ads-execute-sql-direct/)

---

[AdsStmtSetTableReadOnly →]({{ site.baseurl }}/pt/funcoes/ads-stmt-set-table-read-only/)
