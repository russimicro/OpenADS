---
title: AdsIsServerLoaded
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-is-server-loaded/
---

# AdsIsServerLoaded

Verifica se o servidor está carregado.

## Sintaxe

```c
UNSIGNED32 AdsIsServerLoaded(UNSIGNED8* pucServer, UNSIGNED16* pbLoaded);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `pucServer` | `UNSIGNED8*` | Nome do servidor (reservado). |
| `pbLoaded` | `UNSIGNED16*` | Ponteiro para receber 1 se carregado, 0 caso contrário. |

## Valor de Retorno

`AE_SUCCESS` (0) sempre.

## Descrição

`AdsIsServerLoaded` verifica se o servidor está carregado. No OpenADS, retorna sempre 1.

## Exemplo

```c
UNSIGNED16 pbLoaded;
AdsIsServerLoaded(nullptr, &pbLoaded);
// pbLoaded é 1 (servidor carregado)
```

## Ver Também

- [AdsGetServerName]({{ site.baseurl }}/pt/funcoes/ads-get-server-name/)
- [AdsGetVersion]({{ site.baseurl }}/pt/funcoes/ads-get-version/)
- [AdsGetServerTime]({{ site.baseurl }}/pt/funcoes/ads-get-server-time/)

---

[AdsGetServerName →]({{ site.baseurl }}/pt/funcoes/ads-get-server-name/)
