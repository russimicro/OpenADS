---
title: AdsGetIndexName
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-index-name/
---

# AdsGetIndexName

Retorna o nome (tag) de um índice.

## Sintaxe

```c
UNSIGNED32 AdsGetIndexName(ADSHANDLE hIndex, UNSIGNED8* pucBuf,
                           UNSIGNED16* pusBufLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hIndex` | `ADSHANDLE` | Handle do índice. |
| `pucBuf` | `UNSIGNED8*` | Buffer para receber o nome do índice. |
| `pusBufLen` | `UNSIGNED16*` | Ponteiro para o tamanho do buffer. Na saída, contém o comprimento. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsGetIndexName` retorna o nome (tag) de um índice previamente aberto.

## Exemplo

```c
UNSIGNED8 szName[128];
UNSIGNED16 usLen = sizeof(szName);
AdsGetIndexName(hIndex, szName, &usLen);
// szName contém o nome do índice
```

## Ver Também

- [AdsGetIndexHandle]({{ site.baseurl }}/pt/funcoes/ads-get-index-handle/)
- [AdsGetIndexFilename]({{ site.baseurl }}/pt/funcoes/ads-get-index-filename/)
- [AdsGetNumIndexes]({{ site.baseurl }}/pt/funcoes/ads-get-num-indexes/)

---

[AdsOpenIndex →]({{ site.baseurl }}/pt/funcoes/ads-open-index/)
