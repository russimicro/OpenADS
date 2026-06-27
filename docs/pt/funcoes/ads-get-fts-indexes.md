---
title: AdsGetFTSIndexes
layout: default
parent: Referência da API
nav_order: 5
permalink: /pt/funcoes/ads-get-fts-indexes/
---

# AdsGetFTSIndexes

Retorna os handles de todos os índices de pesquisa de texto completo (FTS) de uma tabela.

## Sintaxe

```c
UNSIGNED32 AdsGetFTSIndexes(ADSHANDLE hTable, ADSHANDLE* ahIndex,
                            UNSIGNED16* pusArrayLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `ahIndex` | `ADSHANDLE*` | Array de handles de saída para os índices FTS. |
| `pusArrayLen` | `UNSIGNED16*` | Tamanho do array (entrada) e número de índices retornados (saída). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsGetFTSIndexes` retorna os handles de todos os índices de pesquisa de texto completo associados à tabela especificada. É útil para enumerar os índices FTS disponíveis antes de realizar buscas full-text.

## Exemplo

```c
ADSHANDLE ahIndexes[10];
UNSIGNED16 usCount = 10;
AdsGetFTSIndexes(hTable, ahIndexes, &usCount);
// ahIndexes contém os handles dos índices FTS
```

## Ver Também

- [AdsGetAllIndexes]({{ site.baseurl }}/pt/funcoes/ads-get-all-indexes/)
- [AdsCreateFTSIndex]({{ site.baseurl }}/pt/funcoes/ads-create-fts-index/)
- [AdsFTSSearch]({{ site.baseurl }}/pt/funcoes/ads-fts-search/)

---

[AdsGetJulian →]({{ site.baseurl }}/pt/funcoes/ads-get-julian/)
