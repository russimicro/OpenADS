---
title: AdsGetIndexFilename
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-index-filename/
---

# AdsGetIndexFilename

Retorna o nome do arquivo do índice.

## Sintaxe

```c
UNSIGNED32 AdsGetIndexFilename(ADSHANDLE hIndex, UNSIGNED16 usOrder,
                               UNSIGNED8* p, UNSIGNED16* l);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hIndex` | `ADSHANDLE` | Handle do índice. |
| `usOrder` | `UNSIGNED16` | Ordem (reservada). |
| `p` | `UNSIGNED8*` | Buffer para receber o nome do arquivo. |
| `l` | `UNSIGNED16*` | Ponteiro para o tamanho do buffer. Na saída, contém o comprimento. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsGetIndexFilename` retorna o caminho completo do arquivo de índice.

## Exemplo

```c
UNSIGNED8 szFile[256];
UNSIGNED16 usLen = sizeof(szFile);
AdsGetIndexFilename(hIndex, 0, szFile, &usLen);
// szFile contém o caminho do arquivo de índice
```

## Ver Também

- [AdsGetIndexName]({{ site.baseurl }}/pt/funcoes/ads-get-index-name/)
- [AdsGetIndexExpr]({{ site.baseurl }}/pt/funcoes/ads-get-index-expr/)
- [AdsOpenIndex]({{ site.baseurl }}/pt/funcoes/ads-open-index/)

---

[AdsGetIndexHandleByOrder →]({{ site.baseurl }}/pt/funcoes/ads-get-index-handle-by-order/)
