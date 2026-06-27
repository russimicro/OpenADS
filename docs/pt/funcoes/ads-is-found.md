---
title: AdsIsFound
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-is-found/
---

# AdsIsFound

Verifica se a última procura encontrou o registo.

## Sintaxe

```c
UNSIGNED32 AdsIsFound(ADSHANDLE hTable, UNSIGNED16* pbFound);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pbFound` | `UNSIGNED16*` | Ponteiro para receber 1 se encontrou, 0 caso contrário. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsIsFound` verifica se a última operação de procura (`AdsSeek` ou `AdsSeekLast`) encontrou o registo procurado.

Para tabelas remotas, o estado é servido do cache quando disponível, poupando um round-trip.

## Exemplo

```c
AdsSeek(hIndex, "Silva", 5, ADS_STRINGKEY, ADS_HARDSEEK, nullptr);
UNSIGNED16 bFound;
AdsIsFound(hTable, &bFound);
if (bFound) {
    // A procura encontrou o registo
}
```

## Ver Também

- [AdsSeek]({{ site.baseurl }}/pt/funcoes/ads-seek/)
- [AdsSeekLast]({{ site.baseurl }}/pt/funcoes/ads-seek-last/)
- [AdsGetKeyCount]({{ site.baseurl }}/pt/funcoes/ads-get-key-count/)

---

[AdsExtractKey →]({{ site.baseurl }}/pt/funcoes/ads-extract-key/)
