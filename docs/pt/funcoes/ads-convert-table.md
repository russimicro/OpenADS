---
title: AdsConvertTable
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-convert-table/
---

# AdsConvertTable

Converte uma tabela para outro formato.

## Sintaxe

```c
UNSIGNED32 AdsConvertTable(ADSHANDLE   hHandle,
                           UNSIGNED16  usFilterOption,
                           UNSIGNED8*  pucFile,
                           UNSIGNED16  usTargetType);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hHandle` | `ADSHANDLE` | Handle da tabela. |
| `usFilterOption` | `UNSIGNED16` | Opção de filtro (reservada). |
| `pucFile` | `UNSIGNED8*` | Nome do arquivo de destino. |
| `usTargetType` | `UNSIGNED16` | Tipo de destino (reservado). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsConvertTable` converte uma tabela para outro formato. Atualmente, a função apenas copia a tabela no mesmo formato.

## Exemplo

```c
AdsConvertTable(hTable, 0, "convertida.dbf", ADS_CDX);
```

## Ver Também

- [AdsCopyTable]({{ site.baseurl }}/pt/funcoes/ads-copy-table/)
- [AdsRestructureTable]({{ site.baseurl }}/pt/funcoes/ads-restructure-table/)
- [AdsCreateTable]({{ site.baseurl }}/pt/funcoes/ads-create-table/)

---

[AdsRestructureTable →]({{ site.baseurl }}/pt/funcoes/ads-restructure-table/)
