---
title: AdsSetShort
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-set-short/
---

# AdsSetShort

Define o valor de um campo como inteiro curto.

## Sintaxe

```c
UNSIGNED32 AdsSetShort(ADSHANDLE hObj, UNSIGNED8* pId, SIGNED32 sValue);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hObj` | `ADSHANDLE` | Handle da tabela ou statement. |
| `pId` | `UNSIGNED8*` | Nome do campo ou ordinal. |
| `sValue` | `SIGNED32` | Valor inteiro a definir. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsSetShort` define o valor de um campo como inteiro curto. Internamente, converte para double.

## Exemplo

```c
AdsSetShort(hTable, "Codigo", 123);
AdsWriteRecord(hTable);
```

## Ver Também

- [AdsSetDouble]({{ site.baseurl }}/pt/funcoes/ads-set-double/)
- [AdsSetLongLong]({{ site.baseurl }}/pt/funcoes/ads-set-long-long/)
- [AdsSetMoney]({{ site.baseurl }}/pt/funcoes/ads-set-money/)

---

[AdsSetMoney →]({{ site.baseurl }}/pt/funcoes/ads-set-money/)
