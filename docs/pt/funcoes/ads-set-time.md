---
title: AdsSetTime
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-set-time/
---

# AdsSetTime

Define o valor de um campo de hora.

## Sintaxe

```c
UNSIGNED32 AdsSetTime(ADSHANDLE hObj, UNSIGNED8* pId, UNSIGNED8* pucValue,
                      UNSIGNED16 usLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hObj` | `ADSHANDLE` | Handle da tabela ou statement. |
| `pId` | `UNSIGNED8*` | Nome do campo ou ordinal. |
| `pucValue` | `UNSIGNED8*` | Valor da hora. |
| `usLen` | `UNSIGNED16` | Comprimento do valor. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsSetTime` define o valor de um campo de hora.

## Exemplo

```c
AdsSetTime(hTable, "Hora", "14:30:00", 8);
AdsWriteRecord(hTable);
```

## Ver Também

- [AdsSetTimeStamp]({{ site.baseurl }}/pt/funcoes/ads-set-timestamp/)
- [AdsSetString]({{ site.baseurl }}/pt/funcoes/ads-set-string/)
- [AdsSetField]({{ site.baseurl }}/pt/funcoes/ads-set-field/)

---

[AdsSetTimeStamp →]({{ site.baseurl }}/pt/funcoes/ads-set-timestamp/)
