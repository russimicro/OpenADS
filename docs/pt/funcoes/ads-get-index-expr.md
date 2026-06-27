---
title: AdsGetIndexExpr
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-index-expr/
---

# AdsGetIndexExpr

Retorna a expressão do índice.

## Sintaxe

```c
UNSIGNED32 AdsGetIndexExpr(ADSHANDLE hIndex, UNSIGNED8* pucBuf,
                           UNSIGNED16* pusBufLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hIndex` | `ADSHANDLE` | Handle do índice. |
| `pucBuf` | `UNSIGNED8*` | Buffer para receber a expressão. |
| `pusBufLen` | `UNSIGNED16*` | Ponteiro para o tamanho do buffer. Na saída, contém o comprimento. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsGetIndexExpr` retorna a expressão do índice (ex: "Nome", "Data + 30").

## Exemplo

```c
UNSIGNED8 szExpr[256];
UNSIGNED16 usLen = sizeof(szExpr);
AdsGetIndexExpr(hIndex, szExpr, &usLen);
// szExpr contém a expressão do índice
```

## Ver Também

- [AdsGetIndexName]({{ site.baseurl }}/pt/funcoes/ads-get-index-name/)
- [AdsGetIndexFilename]({{ site.baseurl }}/pt/funcoes/ads-get-index-filename/)
- [AdsGetIndexCondition]({{ site.baseurl }}/pt/funcoes/ads-get-index-condition/)

---

[AdsGetIndexCondition →]({{ site.baseurl }}/pt/funcoes/ads-get-index-condition/)
