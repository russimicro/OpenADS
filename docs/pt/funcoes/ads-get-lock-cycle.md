---
title: AdsGetLockCycle
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-lock-cycle/
---

# AdsGetLockCycle

Retorna o ciclo de bloqueio em milissegundos.

## Sintaxe

```c
UNSIGNED32 AdsGetLockCycle(ADSHANDLE hConnect, UNSIGNED32* pulCycle);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `pulCycle` | `UNSIGNED32*` | Ponteiro para receber o ciclo em ms. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsGetLockCycle` retorna o ciclo de bloqueio em milissegundos.

## Exemplo

```c
UNSIGNED32 ulCycle;
AdsGetLockCycle(hConnect, &ulCycle);
// ulCycle contém o ciclo em ms
```

## Ver Também

- [AdsSetLockCycle]({{ site.baseurl }}/pt/funcoes/ads-set-lock-cycle/)
- [AdsGetLockRetryCount]({{ site.baseurl }}/pt/funcoes/ads-get-lock-retry-count/)
- [AdsSetLockRetryCount]({{ site.baseurl }}/pt/funcoes/ads-set-lock-retry-count/)

---

[AdsSetLockCycle →]({{ site.baseurl }}/pt/funcoes/ads-set-lock-cycle/)
