---
title: AdsGetKeyCount
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-key-count/
---

# AdsGetKeyCount

Retorna o número de chaves no índice.

## Sintaxe

```c
UNSIGNED32 AdsGetKeyCount(ADSHANDLE hIndex, UNSIGNED16 usFilter,
                          UNSIGNED32* pulCount);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hIndex` | `ADSHANDLE` | Handle do índice ou tabela. |
| `usFilter` | `UNSIGNED16` | Opção de filtro (reservada). |
| `pulCount` | `UNSIGNED32*` | Ponteiro para receber o número de chaves. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsGetKeyCount` retorna o número de chaves no índice. Para índices condicionais (FOR), o número de chaves pode ser menor que o número de registos da tabela.

Para índices CDX, a função usa a cache do walk ordenado para retorno O(1).

## Exemplo

```c
UNSIGNED32 ulCount;
AdsGetKeyCount(hIndex, 0, &ulCount);
// ulCount contém o número de chaves no índice
```

## Ver Também

- [AdsGetRecordCount]({{ site.baseurl }}/pt/funcoes/ads-get-record-count/)
- [AdsGetKeyLength]({{ site.baseurl }}/pt/funcoes/ads-get-key-length/)
- [AdsGetKeyType]({{ site.baseurl }}/pt/funcoes/ads-get-key-type/)

---

[AdsGetKeyLength →]({{ site.baseurl }}/pt/funcoes/ads-get-key-length/)
