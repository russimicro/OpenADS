---
title: AdsDisableLocalConnections
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-disable-local-connections/
---

# AdsDisableLocalConnections

Desativa as conexões locais.

## Sintaxe

```c
UNSIGNED32 AdsDisableLocalConnections(void);
```

## Parâmetros

Nenhum.

## Valor de Retorno

`AE_SUCCESS` (0) sempre.

## Descrição

`AdsDisableLocalConnections` desativa as conexões locais. No OpenADS, é uma operação de no-op.

## Exemplo

```c
AdsDisableLocalConnections();
```

## Ver Também

- [AdsEnableRI]({{ site.baseurl }}/pt/funcoes/ads-enable-ri/)
- [AdsDisableRI]({{ site.baseurl }}/pt/funcoes/ads-disable-ri/)
- [AdsConnect]({{ site.baseurl }}/pt/funcoes/ads-connect/)

---

[AdsEnableRI →]({{ site.baseurl }}/pt/funcoes/ads-enable-ri/)
