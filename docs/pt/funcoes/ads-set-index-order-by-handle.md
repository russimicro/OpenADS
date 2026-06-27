---
title: AdsSetIndexOrderByHandle
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-set-index-order-by-handle/
---

# AdsSetIndexOrderByHandle

Define a ordem ativa pelo handle do índice.

## Sintaxe

```c
UNSIGNED32 AdsSetIndexOrderByHandle(ADSHANDLE hTable, ADSHANDLE hIndex);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `hIndex` | `ADSHANDLE` | Handle do índice. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsSetIndexOrderByHandle` define qual índice está ativo pelo seu handle.

## Exemplo

```c
AdsSetIndexOrderByHandle(hTable, hIndex);
```

## Ver Também

- [AdsSetIndexOrder]({{ site.baseurl }}/pt/funcoes/ads-set-index-order/)
- [AdsGetIndexHandle]({{ site.baseurl }}/pt/funcoes/ads-get-index-handle/)
- [AdsGetIndexOrderByHandle]({{ site.baseurl }}/pt/funcoes/ads-get-index-order-by-handle/)

---

[AdsSetProperty90 →]({{ site.baseurl }}/pt/funcoes/ads-set-property-90/)
