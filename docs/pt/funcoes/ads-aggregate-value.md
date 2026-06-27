---
title: AdsAggregateValue
layout: default
parent: Referência da API
nav_order: 5
permalink: /pt/funcoes/ads-aggregate-value/
---

# AdsAggregateValue

Retorna o valor de um item de agregação.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsAggregateValue(ADSHANDLE   hRes,
                                         UNSIGNED32  ulIndex,
                                         UNSIGNED16* pusType,
                                         UNSIGNED8*  pucBuf,
                                         UNSIGNED16* pusLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hRes` | `ADSHANDLE` | Handle do resultado de agregação. |
| `ulIndex` | `UNSIGNED32` | Índice do item (base 0). |
| `pusType` | `UNSIGNED16*` | Tipo do valor: 0=vazio/nulo, 1=numérico, 2=string. |
| `pucBuf` | `UNSIGNED8*` | Buffer para receber o valor. |
| `pusLen` | `UNSIGNED16*` | Capacidade do buffer / bytes escritos. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsAggregateValue` retorna o valor de um item de agregação específico. O tipo é retornado em `pusType`: 0 para vazio/nulo, 1 para numérico (ASCII decimal), 2 para string (bytes brutos do campo).

## Exemplo

```c
UNSIGNED16 usType;
UNSIGNED16 usLen = 255;
UNSIGNED8 aucBuf[256];
AdsAggregateValue(hResult, 0, &usType, aucBuf, &usLen);
```

## Ver Também

- [AdsAggregate]({{ site.baseurl }}/pt/funcoes/ads-aggregate/)
- [AdsAggregateCount]({{ site.baseurl }}/pt/funcoes/ads-aggregate-count/)
- [AdsAggregateClose]({{ site.baseurl }}/pt/funcoes/ads-aggregate-close/)

---

[AdsBinaryToFile →]({{ site.baseurl }}/pt/funcoes/ads-binary-to-file/)
