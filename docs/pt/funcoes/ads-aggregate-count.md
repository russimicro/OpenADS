---
title: AdsAggregateCount
layout: default
parent: Referência da API
nav_order: 4
permalink: /pt/funcoes/ads-aggregate-count/
---

# AdsAggregateCount

Retorna o número de itens de agregação no resultado.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsAggregateCount(ADSHANDLE hRes, UNSIGNED32* pulCount);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hRes` | `ADSHANDLE` | Handle do resultado de agregação. |
| `pulCount` | `UNSIGNED32*` | Ponteiro para receber o número de itens. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsAggregateCount` retorna o número de itens de agregação no resultado retornado por `AdsAggregate`. Cada item corresponde a uma especificação na lista `pszAggSpec`.

## Exemplo

```c
UNSIGNED32 ulCount;
AdsAggregateCount(hResult, &ulCount);
```

## Ver Também

- [AdsAggregate]({{ site.baseurl }}/pt/funcoes/ads-aggregate/)
- [AdsAggregateValue]({{ site.baseurl }}/pt/funcoes/ads-aggregate-value/)
- [AdsAggregateClose]({{ site.baseurl }}/pt/funcoes/ads-aggregate-close/)

---

[AdsAggregateValue →]({{ site.baseurl }}/pt/funcoes/ads-aggregate-value/)
