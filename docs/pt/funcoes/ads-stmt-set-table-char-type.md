---
title: AdsStmtSetTableCharType
layout: default
parent: Referência da API
nav_order: 43
permalink: /pt/funcoes/ads-stmt-set-table-char-type/
---

# AdsStmtSetTableCharType

Define o tipo de caracteres para tabelas acessadas por uma instrução SQL.

## Sintaxe

```c
UNSIGNED32 AdsStmtSetTableCharType(ADSHANDLE hStatement, UNSIGNED16 usCharType);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hStatement` | `ADSHANDLE` | Handle da instrução SQL. |
| `usCharType` | `UNSIGNED16` | Tipo de caracteres (ADS_ANSI ou ADS_OEM). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsStmtSetTableCharType` define o tipo de caracteres que será usado para tabelas acessadas pela instrução SQL. `ADS_ANSI` (1) usa a tabela de caracteres ANSI, enquanto `ADS_OEM` (2) usa a tabela de caracteres OEM.

## Exemplo

```c
AdsStmtSetTableCharType(hStatement, ADS_ANSI);
```

## Ver Também

- [AdsStmtSetTableCollation]({{ site.baseurl }}/pt/funcoes/ads-stmt-set-table-collation/)
- [AdsGetTableCharType]({{ site.baseurl }}/pt/funcoes/ads-get-table-char-type/)
- [AdsExecuteSQL]({{ site.baseurl }}/pt/funcoes/ads-execute-sql/)

---

[AdsStmtSetTableCollation →]({{ site.baseurl }}/pt/funcoes/ads-stmt-set-table-collation/)
