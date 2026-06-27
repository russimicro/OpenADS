---
title: AdsGetIndexCondition
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-index-condition/
---

# AdsGetIndexCondition

Retorna a condição FOR do índice.

## Sintaxe

```c
UNSIGNED32 AdsGetIndexCondition(ADSHANDLE hIndex, UNSIGNED8* p,
                               UNSIGNED16* l);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hIndex` | `ADSHANDLE` | Handle do índice. |
| `p` | `UNSIGNED8*` | Buffer para receber a condição. |
| `l` | `UNSIGNED16*` | Ponteiro para o tamanho do buffer. Na saída, contém o comprimento. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsGetIndexCondition` retorna a condição FOR do índice (se houver).

## Exemplo

```c
UNSIGNED8 szCond[256];
UNSIGNED16 usLen = sizeof(szCond);
AdsGetIndexCondition(hIndex, szCond, &usLen);
// szCond contém a condição FOR
```

## Ver Também

- [AdsGetIndexExpr]({{ site.baseurl }}/pt/funcoes/ads-get-index-expr/)
- [AdsGetIndexName]({{ site.baseurl }}/pt/funcoes/ads-get-index-name/)
- [AdsGetIndexFilename]({{ site.baseurl }}/pt/funcoes/ads-get-index-filename/)

---

[AdsGetIndexFilename →]({{ site.baseurl }}/pt/funcoes/ads-get-index-filename/)
