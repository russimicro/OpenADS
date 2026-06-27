---
title: AdsSetIndexDirection
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-set-index-direction/
---

# AdsSetIndexDirection

Define a direção de navegação do índice.

## Sintaxe

```c
UNSIGNED32 AdsSetIndexDirection(ADSHANDLE hIndex, UNSIGNED16 usDir);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hIndex` | `ADSHANDLE` | Handle do índice. |
| `usDir` | `UNSIGNED16` | Direção: 0 (ADS_ASCENDING) ou 1 (ADS_DESCENDING). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsSetIndexDirection` define a direção de navegação do índice.

## Exemplo

```c
AdsSetIndexDirection(hIndex, ADS_DESCENDING);  // Ordem decrescente
AdsSetIndexDirection(hIndex, ADS_ASCENDING);   // Ordem crescente
```

## Ver Também

- [AdsSetIndexOrder]({{ site.baseurl }}/pt/funcoes/ads-set-index-order/)
- [AdsGetIndexOrderByHandle]({{ site.baseurl }}/pt/funcoes/ads-get-index-order-by-handle/)
- [AdsSeek]({{ site.baseurl }}/pt/funcoes/ads-seek/)

---

[AdsGetLastTableUpdate →]({{ site.baseurl }}/pt/funcoes/ads-get-last-table-update/)
