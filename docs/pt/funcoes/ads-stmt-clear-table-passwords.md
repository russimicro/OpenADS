---
title: AdsStmtClearTablePasswords
layout: default
parent: Referência da API
nav_order: 41
permalink: /pt/funcoes/ads-stmt-clear-table-passwords/
---

# AdsStmtClearTablePasswords

Remove as senhas de tabela associadas a uma instrução SQL.

## Sintaxe

```c
UNSIGNED32 AdsStmtClearTablePasswords(ADSHANDLE hStatement);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hStatement` | `ADSHANDLE` | Handle da instrução SQL. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsStmtClearTablePasswords` remove todas as senhas de tabela que foram previamente associadas à instrução SQL usando `AdsStmtSetTablePassword`.

## Exemplo

```c
AdsStmtClearTablePasswords(hStatement);
// Senhas de tabela removidas da instrução
```

## Ver Também

- [AdsStmtSetTablePassword]({{ site.baseurl }}/pt/funcoes/ads-stmt-set-table-password/)
- [AdsCreateSQLStatement]({{ site.baseurl }}/pt/funcoes/ads-create-sql-statement/)
- [AdsPrepareSQL]({{ site.baseurl }}/pt/funcoes/ads-prepare-sql/)

---

[AdsStmtDisableEncryption →]({{ site.baseurl }}/pt/funcoes/ads-stmt-disable-encryption/)
