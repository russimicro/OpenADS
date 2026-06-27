---
title: AdsGetAllIndexes
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-all-indexes/
---

# AdsGetAllIndexes

Retorna os handles de todos os índices abertos.

## Sintaxe

```c
UNSIGNED32 AdsGetAllIndexes(ADSHANDLE hTable, ADSHANDLE* ahIndex,
                            UNSIGNED16* pusArrayLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `ahIndex` | `ADSHANDLE*` | Array para receber os handles dos índices. |
| `pusArrayLen` | `UNSIGNED16*` | Tamanho do array. Na saída, contém o número de índices. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsGetAllIndexes` retorna os handles de todos os índices atualmente abertos para a tabela especificada.

**Nota:** Para tabelas remotas, a função retorna 0 (os handles de índices remotos não são enumerados desta forma).

## Exemplo

```c
ADSHANDLE arr[64];
UNSIGNED16 usLen = 64;
AdsGetAllIndexes(hTable, arr, &usLen);
// arr contém os handles dos índices abertos
```

## Ver También

- [AdsGetNumIndexes]({{ site.baseurl }}/pt/funcoes/ads-get-num-indexes/)
- [AdsOpenIndex]({{ site.baseurl }}/pt/funcoes/ads-open-index/)
- [AdsGetIndexHandle]({{ site.baseurl }}/pt/funcoes/ads-get-index-handle/)

---

[AdsGetAllTables →]({{ site.baseurl }}/pt/funcoes/ads-get-all-tables/)
