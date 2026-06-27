---
title: AdsGetKeyLength
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-key-length/
---

# AdsGetKeyLength

Retorna o comprimento da chave do índice.

## Sintaxe

```c
UNSIGNED32 AdsGetKeyLength(ADSHANDLE hIndex, UNSIGNED16* p);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hIndex` | `ADSHANDLE` | Handle do índice. |
| `p` | `UNSIGNED16*` | Ponteiro para receber o comprimento da chave. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsGetKeyLength` retorna o comprimento em bytes da chave do índice.

## Exemplo

```c
UNSIGNED16 usKeyLen;
AdsGetKeyLength(hIndex, &usKeyLen);
// usKeyLen contém o comprimento da chave
```

## Ver Também

- [AdsGetKeyType]({{ site.baseurl }}/pt/funcoes/ads-get-key-type/)
- [AdsGetKeyCount]({{ site.baseurl }}/pt/funcoes/ads-get-key-count/)
- [AdsExtractKey]({{ site.baseurl }}/pt/funcoes/ads-extract-key/)

---

[AdsGetKeyType →]({{ site.baseurl }}/pt/funcoes/ads-get-key-type/)
