---
title: AdsGetTableCharType
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-table-char-type/
---

# AdsGetTableCharType

Retorna o tipo de caracteres da tabela.

## Sintaxe

```c
UNSIGNED32 AdsGetTableCharType(ADSHANDLE hTable, UNSIGNED16* p);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `p` | `UNSIGNED16*` | Ponteiro para receber o tipo de caracteres. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsGetTableCharType` retorna o tipo de caracteres da tabela. O OpenADS usa sempre ANSI.

## Exemplo

```c
UNSIGNED16 usCharType;
AdsGetTableCharType(hTable, &usCharType);
// usCharType é ADS_ANSI
```

## Ver Também

- [AdsGetTableType]({{ site.baseurl }}/pt/funcoes/ads-get-table-type/)
- [AdsGetTableAlias]({{ site.baseurl }}/pt/funcoes/ads-get-table-alias/)
- [AdsSetCollation]({{ site.baseurl }}/pt/funcoes/ads-set-collation/)

---

[AdsGetTableConType →]({{ site.baseurl }}/pt/funcoes/ads-get-table-con-type/)
