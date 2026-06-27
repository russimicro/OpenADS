---
title: AdsIsRecordVisible
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-is-record-visible/
---

# AdsIsRecordVisible

Verifica se um registo está visível.

## Sintaxe

```c
UNSIGNED32 AdsIsRecordVisible(ADSHANDLE hObj, UNSIGNED16* pbVisible);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hObj` | `ADSHANDLE` | Handle da tabela. |
| `pbVisible` | `UNSIGNED16*` | Ponteiro para receber 1 se visível, 0 caso contrário. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsIsRecordVisible` verifica se um registo está visível (não eliminado e satisfaz filtros ativos).

**Nota:** No OpenADS, esta função sempre retorna 1 (o registo está visível).

## Exemplo

```c
UNSIGNED16 pbVisible;
AdsIsRecordVisible(hTable, &pbVisible);
// pbVisible indica se o registo está visível
```

## Ver Também

- [AdsIsRecordDeleted]({{ site.baseurl }}/pt/funcoes/ads-is-record-deleted/)
- [AdsIsRecordInAOF]({{ site.baseurl }}/pt/funcoes/ads-is-record-in-aof/)
- [AdsShowDeleted]({{ site.baseurl }}/pt/funcoes/ads-show-deleted/)

---

[AdsLockRecord →]({{ site.baseurl }}/pt/funcoes/ads-lock-record/)
