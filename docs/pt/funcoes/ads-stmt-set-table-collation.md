---
title: AdsStmtSetTableCollation
layout: default
parent: Referência da API
nav_order: 44
permalink: /pt/funcoes/ads-stmt-set-table-collation/
---

# AdsStmtSetTableCollation

Define a colação para tabelas acessadas por uma instrução SQL.

## Sintaxe

```c
UNSIGNED32 AdsStmtSetTableCollation(ADSHANDLE hStatement, UNSIGNED8* pucCollation);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hStatement` | `ADSHANDLE` | Handle da instrução SQL. |
| `pucCollation` | `UNSIGNED8*` | Nome da colação a ser aplicada. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsStmtSetTableCollation` define a colação que será usada para tabelas acessadas pela instrução SQL. A colação afeta a ordenação e comparação de strings.

## Exemplo

```c
AdsStmtSetTableCollation(hStatement, "LATIN1");
```

## Ver Também

- [AdsStmtSetTableCharType]({{ site.baseurl }}/pt/funcoes/ads-stmt-set-table-char-type/)
- [AdsSetCollation]({{ site.baseurl }}/pt/funcoes/ads-set-collation/)
- [AdsExecuteSQL]({{ site.baseurl }}/pt/funcoes/ads-execute-sql/)

---

[AdsStmtSetTableRights →]({{ site.baseurl }}/pt/funcoes/ads-stmt-set-table-rights/)
