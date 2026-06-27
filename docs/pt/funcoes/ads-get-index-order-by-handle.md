---
title: AdsGetIndexOrderByHandle
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-index-order-by-handle/
---

# AdsGetIndexOrderByHandle

Retorna a ordem do índice pelo seu handle.

## Sintaxe

```c
UNSIGNED32 AdsGetIndexOrderByHandle(ADSHANDLE hIndex, UNSIGNED16* p);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hIndex` | `ADSHANDLE` | Handle do índice. |
| `p` | `UNSIGNED16*` | Ponteiro para receber a ordem (1-based). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsGetIndexOrderByHandle` retorna a posição do índice na ordem dos índices abertos (1-based). Retorna 0 para handle desconhecido (ordem natural).

## Exemplo

```c
UNSIGNED16 usOrder;
AdsGetIndexOrderByHandle(hIndex, &usOrder);
// usOrder contém a posição do índice
```

## Ver Também

- [AdsGetIndexHandleByOrder]({{ site.baseurl }}/pt/funcoes/ads-get-index-handle-by-order/)
- [AdsGetIndexName]({{ site.baseurl }}/pt/funcoes/ads-get-index-name/)
- [AdsSetIndexOrder]({{ site.baseurl }}/pt/funcoes/ads-set-index-order/)

---

[AdsGetKeyNum →]({{ site.baseurl }}/pt/funcoes/ads-get-key-num/)
