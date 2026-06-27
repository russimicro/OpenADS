---
title: AdsGetLong
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-long/
---

# AdsGetLong

Retorna o valor de um campo como inteiro de 32 bits.

## Sintaxe

```c
UNSIGNED32 AdsGetLong(ADSHANDLE hTable, UNSIGNED8* pucField, SIGNED32* plVal);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucField` | `UNSIGNED8*` | Nome do campo ou ordinal (via ADSFIELD). |
| `plVal` | `SIGNED32*` | Ponteiro para receber o valor inteiro. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_COLUMN_NOT_FOUND` (6125) se o campo não for encontrado.

## Descrição

`AdsGetLong` retorna o valor de um campo como inteiro de 32 bits. Para campos não numéricos, o valor retornado é 0.

## Exemplo

```c
SIGNED32 lValue;
AdsGetLong(hTable, "Codigo", &lValue);
// lValue contém o valor inteiro
```

## Ver Também

- [AdsGetDouble]({{ site.baseurl }}/pt/funcoes/ads-get-double/)
- [AdsGetLongLong]({{ site.baseurl }}/pt/funcoes/ads-get-long-long/)
- [AdsSetLogical]({{ site.baseurl }}/pt/funcoes/ads-set-logical/)

---

[AdsGetLongLong →]({{ site.baseurl }}/pt/funcoes/ads-get-long-long/)
