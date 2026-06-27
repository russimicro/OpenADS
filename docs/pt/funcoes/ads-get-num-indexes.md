---
title: AdsGetNumIndexes
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-num-indexes/
---

# AdsGetNumIndexes

Retorna o número de índices abertos para uma tabela.

## Sintaxe

```c
UNSIGNED32 AdsGetNumIndexes(ADSHANDLE hTable, UNSIGNED16* pusCount);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pusCount` | `UNSIGNED16*` | Ponteiro para receber o número de índices. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsGetNumIndexes` retorna o número de índices atualmente abertos para a tabela especificada.

Para tabelas remotas, a operação é executada no servidor.

## Exemplo

```c
UNSIGNED16 usCount;
AdsGetNumIndexes(hTable, &usCount);
// usCount contém o número de índices abertos
```

## Ver Também

- [AdsGetAllIndexes]({{ site.baseurl }}/pt/funcoes/ads-get-all-indexes/)
- [AdsOpenIndex]({{ site.baseurl }}/pt/funcoes/ads-open-index/)
- [AdsCloseIndex]({{ site.baseurl }}/pt/funcoes/ads-close-index/)

---

[AdsGetAllIndexes →]({{ site.baseurl }}/pt/funcoes/ads-get-all-indexes/)
