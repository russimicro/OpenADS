---
title: AdsGetLockRetryCount
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-lock-retry-count/
---

# AdsGetLockRetryCount

Retorna o número de tentativas de bloqueio.

## Sintaxe

```c
UNSIGNED32 AdsGetLockRetryCount(ADSHANDLE hConnect, UNSIGNED16* pusRetryCount);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `pusRetryCount` | `UNSIGNED16*` | Ponteiro para receber o número de tentativas. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsGetLockRetryCount` retorna o número de tentativas de bloqueio.

## Exemplo

```c
UNSIGNED16 usRetry;
AdsGetLockRetryCount(hConnect, &usRetry);
// usRetry contém o número de tentativas
```

## Ver Também

- [AdsSetLockRetryCount]({{ site.baseurl }}/pt/funcoes/ads-set-lock-retry-count/)
- [AdsGetLockCycle]({{ site.baseurl }}/pt/funcoes/ads-get-lock-cycle/)
- [AdsSetLockCycle]({{ site.baseurl }}/pt/funcoes/ads-set-lock-cycle/)

---

[AdsSetLockRetryCount →]({{ site.baseurl }}/pt/funcoes/ads-set-lock-retry-count/)
