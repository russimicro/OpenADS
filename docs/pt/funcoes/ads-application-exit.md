---
title: AdsApplicationExit
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-application-exit/
---

# AdsApplicationExit

Termina a aplicação.

## Sintaxe

```c
UNSIGNED32 AdsApplicationExit(void);
```

## Parâmetros

Nenhum.

## Valor de Retorno

`AE_SUCCESS` (0) sempre.

## Descrição

`AdsApplicationExit` notifica o engine que a aplicação está a terminar. No OpenADS, é uma operação de no-op.

## Exemplo

```c
AdsApplicationExit();
```

## Ver Também

- [AdsCloseAllTables]({{ site.baseurl }}/pt/funcoes/ads-close-all-tables/)
- [AdsDisconnect]({{ site.baseurl }}/pt/funcoes/ads-disconnect/)
- [AdsThreadExit]({{ site.baseurl }}/pt/funcoes/ads-thread-exit/)

---

[AdsThreadExit →]({{ site.baseurl }}/pt/funcoes/ads-thread-exit/)
