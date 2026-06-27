---
title: AdsCloseIndex
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-close-index/
---

# AdsCloseIndex

Fecha um índice aberto.

## Sintaxe

```c
UNSIGNED32 AdsCloseIndex(ADSHANDLE hIndex);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hIndex` | `ADSHANDLE` | Handle do índice. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsCloseIndex` fecha um índice previamente aberto e liberta os recursos associados.

## Exemplo

```c
AdsCloseIndex(hIndex);
```

## Ver Também

- [AdsOpenIndex]({{ site.baseurl }}/pt/funcoes/ads-open-index/)
- [AdsCloseAllIndexes]({{ site.baseurl }}/pt/funcoes/ads-close-all-indexes/)
- [AdsGetIndexHandle]({{ site.baseurl }}/pt/funcoes/ads-get-index-handle/)

---

[AdsCloseAllIndexes →]({{ site.baseurl }}/pt/funcoes/ads-close-all-indexes/)
