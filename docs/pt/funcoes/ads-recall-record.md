---
title: AdsRecallRecord
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-recall-record/
---

# AdsRecallRecord

Restaura um registo eliminado.

## Sintaxe

```c
UNSIGNED32 AdsRecallRecord(ADSHANDLE hTable);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsRecallRecord` restaura o registo atual que foi previamente marcado como eliminado com `AdsDeleteRecord`. O registo volta a ficar visível e acessível.

## Exemplo

```c
AdsDeleteRecord(hTable);
// Oregisto está eliminado
AdsRecallRecord(hTable);
// O registo está restaurado
```

## Ver Também

- [AdsDeleteRecord]({{ site.baseurl }}/pt/funcoes/ads-delete-record/)
- [AdsIsRecordDeleted]({{ site.baseurl }}/pt/funcoes/ads-is-record-deleted/)
- [AdsRecallAllRecords]({{ site.baseurl }}/pt/funcoes/ads-recall-all-records/)

---

[AdsIsRecordDeleted →]({{ site.baseurl }}/pt/funcoes/ads-is-record-deleted/)
