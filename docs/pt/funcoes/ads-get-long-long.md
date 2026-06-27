---
title: AdsGetLongLong
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-long-long/
---

# AdsGetLongLong

Retorna o valor de um campo como inteiro de 64 bits.

## Sintaxe

```c
UNSIGNED32 AdsGetLongLong(ADSHANDLE hTable, UNSIGNED8* pucField,
                          std::int64_t* pllValue);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucField` | `UNSIGNED8*` | Nome do campo ou ordinal (via ADSFIELD). |
| `pllValue` | `std::int64_t*` | Ponteiro para receber o valor inteiro de 64 bits. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_COLUMN_NOT_FOUND` (6125) se o campo não for encontrado.

## Descrição

`AdsGetLongLong` retorna o valor de um campo como inteiro de 64 bits. Para campos não numéricos, o valor retornado é 0.

## Exemplo

```c
std::int64_t llValue;
AdsGetLongLong(hTable, "CodigoGrande", &llValue);
// llValue contém o valor inteiro de 64 bits
```

## Ver Também

- [AdsGetLong]({{ site.baseurl }}/pt/funcoes/ads-get-long/)
- [AdsGetDouble]({{ site.baseurl }}/pt/funcoes/ads-get-double/)
- [AdsSetLongLong]({{ site.baseurl }}/pt/funcoes/ads-set-long-long/)

---

[AdsSetLongLong →]({{ site.baseurl }}/pt/funcoes/ads-set-long-long/)
