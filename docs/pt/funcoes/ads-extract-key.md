---
title: AdsExtractKey
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-extract-key/
---

# AdsExtractKey

Retorna a chave do registo atual para o índice.

## Sintaxe

```c
UNSIGNED32 AdsExtractKey(ADSHANDLE hIndex, UNSIGNED8* pucBuf,
                         UNSIGNED16* pusLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hIndex` | `ADSHANDLE` | Handle do índice. |
| `pucBuf` | `UNSIGNED8*` | Buffer para receber a chave. |
| `pusLen` | `UNSIGNED16*` | Ponteiro para o tamanho do buffer. Na saída, contém o comprimento da chave. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsExtractKey` retorna a chave do registo atual para o índice especificado. A chave é avaliada a partir da expressão do índice.

## Exemplo

```c
UNSIGNED8 szKey[256];
UNSIGNED16 usLen = sizeof(szKey);
AdsExtractKey(hIndex, szKey, &usLen);
// szKey contém a chave do registo atual
```

## Ver Também

- [AdsGetKeyLength]({{ site.baseurl }}/pt/funcoes/ads-get-key-length/)
- [AdsGetKeyType]({{ site.baseurl }}/pt/funcoes/ads-get-key-type/)
- [AdsSeek]({{ site.baseurl }}/pt/funcoes/ads-seek/)

---

[AdsGetKeyCount →]({{ site.baseurl }}/pt/funcoes/ads-get-key-count/)
