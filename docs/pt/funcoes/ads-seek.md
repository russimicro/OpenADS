---
title: AdsSeek
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-seek/
---

# AdsSeek

Procura uma chave no índice.

## Sintaxe

```c
UNSIGNED32 AdsSeek(ADSHANDLE hIndex,
                   UNSIGNED8* pucKey,
                   UNSIGNED16 u16KeyLen,
                   UNSIGNED16 u16KeyType,
                   UNSIGNED16 u16SeekType,
                   UNSIGNED16* pbFound);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hIndex` | `ADSHANDLE` | Handle do índice. |
| `pucKey` | `UNSIGNED8*` | Chave a procurar. |
| `u16KeyLen` | `UNSIGNED16` | Comprimento da chave. |
| `u16KeyType` | `UNSIGNED16` | Tipo da chave: `ADS_STRINGKEY` para texto, `ADS_DOUBLEKEY` para numérico. |
| `u16SeekType` | `UNSIGNED16` | Tipo de procura: `ADS_SOFTSEEK` para soft seek, `ADS_HARDSEEK` para hard seek. |
| `pbFound` | `UNSIGNED16*` | Ponteiro para receber 1 se encontrou, 0 caso contrário. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsSeek` procura uma chave no índice e posiciona o cursor no registo correspondente. Se a chave não for encontrada exatamente:
- **Hard seek**: cursor fica no EOF
- **Soft seek**: cursor fica no primeiro registo maior que a chave

## Exemplo

```c
UNSIGNED16 bFound;
AdsSeek(hIndex, "Silva", 5, ADS_STRINGKEY, ADS_HARDSEEK, &bFound);
if (bFound) {
    // Encontrou o registo exacto
}
```

## Ver Também

- [AdsSeekLast]({{ site.baseurl }}/pt/funcoes/ads-seek-last/)
- [AdsIsFound]({{ site.baseurl }}/pt/funcoes/ads-is-found/)
- [AdsExtractKey]({{ site.baseurl }}/pt/funcoes/ads-extract-key/)

---

[AdsSeekLast →]({{ site.baseurl }}/pt/funcoes/ads-seek-last/)
