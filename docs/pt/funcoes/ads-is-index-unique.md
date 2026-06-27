---
title: AdsIsIndexUnique
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-is-index-unique/
---

# AdsIsIndexUnique

Verifica se o índice é único.

## Sintaxe

```c
UNSIGNED32 AdsIsIndexUnique(ADSHANDLE hIndex, UNSIGNED16* pbUnique);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hIndex` | `ADSHANDLE` | Handle do índice. |
| `pbUnique` | `UNSIGNED16*` | Ponteiro para receber 1 se único, 0 caso contrário. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsIsIndexUnique` verifica se o índice é único.

## Exemplo

```c
UNSIGNED16 pbUnique;
AdsIsIndexUnique(hIndex, &pbUnique);
// pbUnique indica se o índice é único
```

## Ver Também

- [AdsIsIndexCustom]({{ site.baseurl }}/pt/funcoes/ads-is-index-custom/)
- [AdsIsIndexDescending]({{ site.baseurl }}/pt/funcoes/ads-is-index-descending/)
- [AdsCreateIndex]({{ site.baseurl }}/pt/funcoes/ads-create-index/)

---

[AdsIsNull →]({{ site.baseurl }}/pt/funcoes/ads-is-null/)
