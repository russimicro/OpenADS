---
title: AdsFetchWhereApplyRow
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-fetch-where-apply-row/
---

# AdsFetchWhereApplyRow

Aplica uma linha do resultado à cache da tabela.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsFetchWhereApplyRow(ADSHANDLE hRes, UNSIGNED32 ulRow, ADSHANDLE hTbl);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hRes` | `ADSHANDLE` | Handle do resultado retornado por AdsFetchWhere. |
| `ulRow` | `UNSIGNED32` | Índice da linha (base 0) a ser aplicada. |
| `hTbl` | `ADSHANDLE` | Handle da tabela de destino. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsFetchWhereApplyRow` carrega os dados de uma linha do resultado (recno e campos) para a cache da tabela, permitindo que AdsGetField, AdsGetRecordNum e AdsAtEOF sirvam esses dados sem uma nova ida ao servidor.

## Exemplo

```c
AdsFetchWhereApplyRow(hResult, 0, hTable);
// Agora AdsGetField funciona localmente
AdsGetField(hTable, "Nome", aucBuffer, &ulLen, ADS_STRING);
```

## Ver Também

- [AdsFetchWhere]({{ site.baseurl }}/pt/funcoes/ads-fetch-where/)
- [AdsFetchWhereRecno]({{ site.baseurl }}/pt/funcoes/ads-fetch-where-recno/)

---

[AdsFetchWhereClose →]({{ site.baseurl }}/pt/funcoes/ads-fetch-where-close/)
