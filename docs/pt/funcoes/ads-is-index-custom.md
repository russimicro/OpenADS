---
title: AdsIsIndexCustom
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-is-index-custom/
---

# AdsIsIndexCustom

Verifica se o índice é personalizado.

## Sintaxe

```c
UNSIGNED32 AdsIsIndexCustom(ADSHANDLE hIndex, UNSIGNED16* pbCustom);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hIndex` | `ADSHANDLE` | Handle do índice. |
| `pbCustom` | `UNSIGNED16*` | Ponteiro para receber 1 se personalizado, 0 caso contrário. |

## Valor de Retorno

`AE_SUCCESS` (0) sempre.

## Descrição

`AdsIsIndexCustom` verifica se o índice é personalizado. No OpenADS, retorna sempre 0.

## Exemplo

```c
UNSIGNED16 pbCustom;
AdsIsIndexCustom(hIndex, &pbCustom);
// pbCustom é 0 (índice não personalizado)
```

## Ver Também

- [AdsIsIndexDescending]({{ site.baseurl }}/pt/funcoes/ads-is-index-descending/)
- [AdsIsIndexUnique]({{ site.baseurl }}/pt/funcoes/ads-is-index-unique/)
- [AdsCreateIndex]({{ site.baseurl }}/pt/funcoes/ads-create-index/)

---

[AdsIsIndexDescending →]({{ site.baseurl }}/pt/funcoes/ads-is-index-descending/)
