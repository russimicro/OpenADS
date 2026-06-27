---
title: AdsGetKeyType
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-key-type/
---

# AdsGetKeyType

Retorna o tipo da chave do índice.

## Sintaxe

```c
UNSIGNED32 AdsGetKeyType(ADSHANDLE hIndex, UNSIGNED16* p);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hIndex` | `ADSHANDLE` | Handle do índice. |
| `p` | `UNSIGNED16*` | Ponteiro para receber o tipo da chave. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsGetKeyType` retorna o tipo da chave do índice:
- `ADS_STRINGKEY` (0) - Chave de texto
- `ADS_DOUBLEKEY` (1) - Chave numérica

## Exemplo

```c
UNSIGNED16 usKeyType;
AdsGetKeyType(hIndex, &usKeyType);
// usKeyType é ADS_STRINGKEY ou ADS_DOUBLEKEY
```

## Ver Também

- [AdsGetKeyLength]({{ site.baseurl }}/pt/funcoes/ads-get-key-length/)
- [AdsGetKeyCount]({{ site.baseurl }}/pt/funcoes/ads-get-key-count/)
- [AdsExtractKey]({{ site.baseurl }}/pt/funcoes/ads-extract-key/)

---

[AdsGetIndexHandle →]({{ site.baseurl }}/pt/funcoes/ads-get-index-handle/)
