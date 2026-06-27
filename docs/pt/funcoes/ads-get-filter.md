---
title: AdsGetFilter
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-filter/
---

# AdsGetFilter

Retorna a expressão do filtro ativo.

## Sintaxe

```c
UNSIGNED32 AdsGetFilter(ADSHANDLE hTable, UNSIGNED8* p, UNSIGNED16* l);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `p` | `UNSIGNED8*` | Buffer para receber a expressão. |
| `l` | `UNSIGNED16*` | Ponteiro para o tamanho do buffer. Na saída, contém o comprimento. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsGetFilter` retorna a expressão do filtro Clipper atualmente ativo na tabela.

## Exemplo

```c
UNSIGNED8 szFilter[256];
UNSIGNED16 usLen = sizeof(szFilter);
AdsGetFilter(hTable, szFilter, &usLen);
// szFilter contém a expressão do filtro
```

## Ver Também

- [AdsSetFilter]({{ site.baseurl }}/pt/funcoes/ads-set-filter/)
- [AdsClearFilter]({{ site.baseurl }}/pt/funcoes/ads-clear-filter/)
- [AdsGetAOF]({{ site.baseurl }}/pt/funcoes/ads-get-aof/)

---

[AdsSetAOF →]({{ site.baseurl }}/pt/funcoes/ads-set-aof/)
