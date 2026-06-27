---
title: AdsGetDouble
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-double/
---

# AdsGetDouble

Retorna o valor de um campo como double.

## Sintaxe

```c
UNSIGNED32 AdsGetDouble(ADSHANDLE hTable, UNSIGNED8* pucField, double* pdVal);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucField` | `UNSIGNED8*` | Nome do campo ou ordinal (via ADSFIELD). |
| `pdVal` | `double*` | Ponteiro para receber o valor numérico. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_COLUMN_NOT_FOUND` (6125) se o campo não for encontrado.

## Descrição

`AdsGetDouble` retorna o valor de um campo como double. Para campos não numéricos, o valor retornado é 0.

Para tabelas remotas, o valor é servido do cache de linha após a primeira navegação.

## Exemplo

```c
double dPrice;
AdsGetDouble(hTable, "Preco", &dPrice);
// dPrice contém o valor numérico
```

## Ver Também

- [AdsSetDouble]({{ site.baseurl }}/pt/funcoes/ads-set-double/)
- [AdsGetLong]({{ site.baseurl }}/pt/funcoes/ads-get-long/)
- [AdsGetLongLong]({{ site.baseurl }}/pt/funcoes/ads-get-long-long/)

---

[AdsGetLong →]({{ site.baseurl }}/pt/funcoes/ads-get-long/)
