---
title: AdsGetDeleted
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-deleted/
---

# AdsGetDeleted

Retorna se os registos eliminados são visíveis.

## Sintaxe

```c
UNSIGNED32 AdsGetDeleted(UNSIGNED16* p);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `p` | `UNSIGNED16*` | Ponteiro para receber 1 se visíveis, 0 caso contrário. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsGetDeleted` retorna se os registos eliminados são visíveis. Retorna 1 quando os registos eliminados SÃO visíveis (corresponde a SET DELETED OFF no Clipper).

## Exemplo

```c
UNSIGNED16 p;
AdsGetDeleted(&p);
// p é 1 (registos eliminados visíveis) ou 0 (ocultos)
```

## Ver Também

- [AdsShowDeleted]({{ site.baseurl }}/pt/funcoes/ads-show-deleted/)
- [AdsDeleteRecord]({{ site.baseurl }}/pt/funcoes/ads-delete-record/)
- [AdsIsRecordDeleted]({{ site.baseurl }}/pt/funcoes/ads-is-record-deleted/)

---

[AdsGetEpoch →]({{ site.baseurl }}/pt/funcoes/ads-get-epoch/)
