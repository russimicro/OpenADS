---
title: AdsSetTimeStamp
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-set-timestamp/
---

# AdsSetTimeStamp

Define o valor de um campo timestamp.

## Sintaxe

```c
UNSIGNED32 AdsSetTimeStamp(ADSHANDLE hObj, UNSIGNED8* pId, UNSIGNED8* pucBuf,
                           UNSIGNED32 ulLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hObj` | `ADSHANDLE` | Handle da tabela ou statement. |
| `pId` | `UNSIGNED8*` | Nome do campo ou ordinal. |
| `pucBuf` | `UNSIGNED8*` | Valor do timestamp. |
| `ulLen` | `UNSIGNED32` | Comprimento do valor. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsSetTimeStamp` define o valor de um campo timestamp.

## Exemplo

```c
AdsSetTimeStamp(hTable, "DataMod", "20260625143000", 14);
AdsWriteRecord(hTable);
```

## Ver Também

- [AdsSetTime]({{ site.baseurl }}/pt/funcoes/ads-set-time/)
- [AdsSetString]({{ site.baseurl }}/pt/funcoes/ads-set-string/)
- [AdsSetField]({{ site.baseurl }}/pt/funcoes/ads-set-field/)

---

[AdsSetIndexOrderByHandle →]({{ site.baseurl }}/pt/funcoes/ads-set-index-order-by-handle/)
