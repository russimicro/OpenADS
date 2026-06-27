---
title: AdsThreadExit
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-thread-exit/
---

# AdsThreadExit

Notifica a terminação de uma thread.

## Sintaxe

```c
UNSIGNED32 AdsThreadExit(void);
```

## Parâmetros

Nenhum.

## Valor de Retorno

`AE_SUCCESS` (0) sempre.

## Descrição

`AdsThreadExit` notifica o engine que uma thread está a terminar. No OpenADS, é uma operação de no-op.

## Exemplo

```c
AdsThreadExit();
```

## Ver Também

- [AdsApplicationExit]({{ site.baseurl }}/pt/funcoes/ads-application-exit/)
- [AdsCloseAllTables]({{ site.baseurl }}/pt/funcoes/ads-close-all-tables/)
- [AdsDisconnect]({{ site.baseurl }}/pt/funcoes/ads-disconnect/)

---

[AdsDisableLocalConnections →]({{ site.baseurl }}/pt/funcoes/ads-disable-local-connections/)
