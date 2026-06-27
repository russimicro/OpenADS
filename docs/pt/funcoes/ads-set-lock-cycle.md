---
title: AdsSetLockCycle
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-set-lock-cycle/
---

# AdsSetLockCycle

Define o ciclo de bloqueio em milissegundos.

## Sintaxe

```c
UNSIGNED32 AdsSetLockCycle(ADSHANDLE hConnect, UNSIGNED32 ulCycle);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `ulCycle` | `UNSIGNED32` | Ciclo em ms. |

## Valor de Retorno

`AE_SUCCESS` (0) sempre.

## Descrição

`AdsSetLockCycle` define o ciclo de bloqueio em milissegundos.

## Exemplo

```c
AdsSetLockCycle(hConnect, 1000);  // 1 segundo
```

## Ver Também

- [AdsGetLockCycle]({{ site.baseurl }}/pt/funcoes/ads-get-lock-cycle/)
- [AdsGetLockRetryCount]({{ site.baseurl }}/pt/funcoes/ads-get-lock-retry-count/)
- [AdsSetLockRetryCount]({{ site.baseurl }}/pt/funcoes/ads-set-lock-retry-count/)

---

[AdsGetLockRetryCount →]({{ site.baseurl }}/pt/funcoes/ads-get-lock-retry-count/)
