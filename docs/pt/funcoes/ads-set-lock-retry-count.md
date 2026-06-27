---
title: AdsSetLockRetryCount
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-set-lock-retry-count/
---

# AdsSetLockRetryCount

Define o número de tentativas de bloqueio.

## Sintaxe

```c
UNSIGNED32 AdsSetLockRetryCount(ADSHANDLE hConnect, UNSIGNED16 usRetryCount);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `usRetryCount` | `UNSIGNED16` | Número de tentativas. |

## Valor de Retorno

`AE_SUCCESS` (0) sempre.

## Descrição

`AdsSetLockRetryCount` define o número de tentativas de bloqueio.

## Exemplo

```c
AdsSetLockRetryCount(hConnect, 10);
```

## Ver Também

- [AdsGetLockRetryCount]({{ site.baseurl }}/pt/funcoes/ads-get-lock-retry-count/)
- [AdsGetLockCycle]({{ site.baseurl }}/pt/funcoes/ads-get-lock-cycle/)
- [AdsSetLockCycle]({{ site.baseurl }}/pt/funcoes/ads-set-lock-cycle/)

---

[AdsLockRecord →]({{ site.baseurl }}/pt/funcoes/ads-lock-record/)
