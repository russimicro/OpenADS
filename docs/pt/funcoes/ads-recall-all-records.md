---
title: AdsRecallAllRecords
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-recall-all-records/
---

# AdsRecallAllRecords

Restaura todos os registos eliminados.

## Sintaxe

```c
UNSIGNED32 AdsRecallAllRecords(ADSHANDLE hTable);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |

## Valor de Retorno

`AE_SUCCESS` (0) sempre.

## Descrição

`AdsRecallAllRecords` restaura todos os registos eliminados. No OpenADS, é uma operação de no-op.

## Exemplo

```c
AdsRecallAllRecords(hTable);
```

## Ver Também

- [AdsRecallRecord]({{ site.baseurl }}/pt/funcoes/ads-recall-record/)
- [AdsDeleteRecord]({{ site.baseurl }}/pt/funcoes/ads-delete-record/)
- [AdsPackTable]({{ site.baseurl }}/pt/funcoes/ads-pack-table/)

---

[AdsIsRecordVisible →]({{ site.baseurl }}/pt/funcoes/ads-is-record-visible/)
