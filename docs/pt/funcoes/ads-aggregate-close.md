---
title: AdsAggregateClose
layout: default
parent: Referência da API
nav_order: 3
permalink: /pt/funcoes/ads-aggregate-close/
---

# AdsAggregateClose

Fecha um handle de resultado de agregação.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsAggregateClose(ADSHANDLE hRes);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hRes` | `ADSHANDLE` | Handle do resultado de agregação. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsAggregateClose` libera os recursos associados a um handle de resultado de agregação retornado por `AdsAggregate`.

## Exemplo

```c
AdsAggregateClose(hResult);
```

## Ver Também

- [AdsAggregate]({{ site.baseurl }}/pt/funcoes/ads-aggregate/)
- [AdsAggregateCount]({{ site.baseurl }}/pt/funcoes/ads-aggregate-count/)
- [AdsAggregateValue]({{ site.baseurl }}/pt/funcoes/ads-aggregate-value/)

---

[AdsAggregateCount →]({{ site.baseurl }}/pt/funcoes/ads-aggregate-count/)
