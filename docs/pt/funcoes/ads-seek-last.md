---
title: AdsSeekLast
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-seek-last/
---

# AdsSeekLast

Procura a última ocorrência de uma chave no índice.

## Sintaxe

```c
UNSIGNED32 AdsSeekLast(ADSHANDLE hIndex,
                       UNSIGNED8* pucKey,
                       UNSIGNED16 u16KeyLen,
                       UNSIGNED16 u16KeyType,
                       UNSIGNED16* pbFound);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hIndex` | `ADSHANDLE` | Handle do índice. |
| `pucKey` | `UNSIGNED8*` | Chave a procurar. |
| `u16KeyLen` | `UNSIGNED16` | Comprimento da chave. |
| `u16KeyType` | `UNSIGNED16` | Tipo da chave: `ADS_STRINGKEY` para texto, `ADS_DOUBLEKEY` para numérico. |
| `pbFound` | `UNSIGNED16*` | Ponteiro para receber 1 se encontrou, 0 caso contrário. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsSeekLast` procura a última ocorrência de uma chave no índice. Útil para índices não únicos onde múltiplos registos podem ter a mesma chave.

## Exemplo

```c
UNSIGNED16 bFound;
AdsSeekLast(hIndex, "Silva", 5, ADS_STRINGKEY, &bFound);
if (bFound) {
    // Posicionado no último "Silva"
}
```

## Ver Também

- [AdsSeek]({{ site.baseurl }}/pt/funcoes/ads-seek/)
- [AdsIsFound]({{ site.baseurl }}/pt/funcoes/ads-is-found/)
- [AdsExtractKey]({{ site.baseurl }}/pt/funcoes/ads-extract-key/)

---

[AdsIsFound →]({{ site.baseurl }}/pt/funcoes/ads-is-found/)
