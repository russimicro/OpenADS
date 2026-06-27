---
title: AdsSetMoney
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-set-money/
---

# AdsSetMoney

Define o valor de um campo monetário.

## Sintaxe

```c
UNSIGNED32 AdsSetMoney(ADSHANDLE hObj, UNSIGNED8* pId, SIGNED64 qValue);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hObj` | `ADSHANDLE` | Handle da tabela ou statement. |
| `pId` | `UNSIGNED8*` | Nome do campo ou ordinal. |
| `qValue` | `SIGNED64` | Valor monetário (escala ACE: dividir por 10000). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsSetMoney` define o valor de um campo monetário. O valor é dividido por 10000 para conversão.

## Exemplo

```c
AdsSetMoney(hTable, "Preco", 199900);  // 19.99
AdsWriteRecord(hTable);
```

## Ver Também

- [AdsSetDouble]({{ site.baseurl }}/pt/funcoes/ads-set-double/)
- [AdsSetShort]({{ site.baseurl }}/pt/funcoes/ads-set-short/)
- [AdsSetLongLong]({{ site.baseurl }}/pt/funcoes/ads-set-long-long/)

---

[AdsSetTime →]({{ site.baseurl }}/pt/funcoes/ads-set-time/)
