---
title: AdsSetEmpty
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-set-empty/
---

# AdsSetEmpty

Define um campo como vazio.

## Sintaxe

```c
UNSIGNED32 AdsSetEmpty(ADSHANDLE hObj, UNSIGNED8* pId);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hObj` | `ADSHANDLE` | Handle da tabela ou statement. |
| `pId` | `UNSIGNED8*` | Nome do campo ou ordinal. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsSetEmpty` define um campo como vazio (string vazia).

## Exemplo

```c
AdsSetEmpty(hTable, "Observacoes");
AdsWriteRecord(hTable);
```

## Ver Também

- [AdsSetNull]({{ site.baseurl }}/pt/funcoes/ads-set-null/)
- [AdsSetString]({{ site.baseurl }}/pt/funcoes/ads-set-string/)
- [AdsSetField]({{ site.baseurl }}/pt/funcoes/ads-set-field/)

---

[AdsSetShort →]({{ site.baseurl }}/pt/funcoes/ads-set-short/)
