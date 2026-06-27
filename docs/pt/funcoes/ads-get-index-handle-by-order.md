---
title: AdsGetIndexHandleByOrder
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-index-handle-by-order/
---

# AdsGetIndexHandleByOrder

Retorna o handle do índice pela sua ordem.

## Sintaxe

```c
UNSIGNED32 AdsGetIndexHandleByOrder(ADSHANDLE hTable, UNSIGNED16 usOrder,
                                    ADSHANDLE* phIndex);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `usOrder` | `UNSIGNED16` | Ordem do índice (1-based). |
| `phIndex` | `ADSHANDLE*` | Ponteiro para receber o handle do índice. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se não houver índice ativo.

## Descrição

`AdsGetIndexHandleByOrder` retorna o handle de um índice pela sua posição na ordem dos índices abertos.

## Exemplo

```c
ADSHANDLE hIndex;
AdsGetIndexHandleByOrder(hTable, 1, &hIndex);
// hIndex contém o handle do primeiro índice
```

## Ver Também

- [AdsGetIndexHandle]({{ site.baseurl }}/pt/funcoes/ads-get-index-handle/)
- [AdsGetIndexOrderByHandle]({{ site.baseurl }}/pt/funcoes/ads-get-index-order-by-handle/)
- [AdsGetNumIndexes]({{ site.baseurl }}/pt/funcoes/ads-get-num-indexes/)

---

[AdsGetIndexOrderByHandle →]({{ site.baseurl }}/pt/funcoes/ads-get-index-order-by-handle/)
