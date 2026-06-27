---
title: AdsIsRecordDeleted
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-is-record-deleted/
---

# AdsIsRecordDeleted

Verifica se o registo atual está eliminado.

## Sintaxe

```c
UNSIGNED32 AdsIsRecordDeleted(ADSHANDLE hTable, UNSIGNED16* pbDeleted);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pbDeleted` | `UNSIGNED16*` | Ponteiro para receber 1 se o registo estiver eliminado, 0 caso contrário. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsIsRecordDeleted` verifica se o registo atual está marcado como eliminado. Para tabelas remotas, o estado de eliminado é retornado junto com o cache de linha.

## Exemplo

```c
AdsDeleteRecord(hTable);
AdsGotoTop(hTable);
if (pbDeleted) {
    // O registo está eliminado
}
```

## Ver Também

- [AdsDeleteRecord]({{ site.baseurl }}/pt/funcoes/ads-delete-record/)
- [AdsRecallRecord]({{ site.baseurl }}/pt/funcoes/ads-recall-record/)
- [AdsShowDeleted]({{ site.baseurl }}/pt/funcoes/ads-show-deleted/)

---

[AdsShowDeleted →]({{ site.baseurl }}/pt/funcoes/ads-show-deleted/)
