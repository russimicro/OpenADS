---
title: AdsIsIndexDescending
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-is-index-descending/
---

# AdsIsIndexDescending

Verifica se o índice é decrescente.

## Sintaxe

```c
UNSIGNED32 AdsIsIndexDescending(ADSHANDLE hIndex, UNSIGNED16* pbDesc);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hIndex` | `ADSHANDLE` | Handle do índice. |
| `pbDesc` | `UNSIGNED16*` | Ponteiro para receber 1 se decrescente, 0 caso contrário. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsIsIndexDescending` verifica se o índice é decrescente.

## Exemplo

```c
UNSIGNED16 pbDesc;
AdsIsIndexDescending(hIndex, &pbDesc);
// pbDesc indica se o índice é decrescente
```

## Ver Também

- [AdsSetIndexDirection]({{ site.baseurl }}/pt/funcoes/ads-set-index-direction/)
- [AdsIsIndexCustom]({{ site.baseurl }}/pt/funcoes/ads-is-index-custom/)
- [AdsIsIndexUnique]({{ site.baseurl }}/pt/funcoes/ads-is-index-unique/)

---

[AdsIsIndexUnique →]({{ site.baseurl }}/pt/funcoes/ads-is-index-unique/)
