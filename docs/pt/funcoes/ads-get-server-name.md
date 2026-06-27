---
title: AdsGetServerName
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-server-name/
---

# AdsGetServerName

Retorna o nome do servidor.

## Sintaxe

```c
UNSIGNED32 AdsGetServerName(ADSHANDLE hConnect,
                            UNSIGNED8* pucBuf, UNSIGNED16* pusLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `pucBuf` | `UNSIGNED8*` | Buffer para receber o nome. |
| `pusLen` | `UNSIGNED16*` | Ponteiro para o tamanho do buffer. Na saída, contém o comprimento. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsGetServerName` retorna o nome do anfitrião (hostname) do servidor.

## Exemplo

```c
UNSIGNED8 szName[256];
UNSIGNED16 usLen = sizeof(szName);
AdsGetServerName(hConnect, szName, &usLen);
// szName contém o nome do servidor
```

## Ver Também

- [AdsGetServerTime]({{ site.baseurl }}/pt/funcoes/ads-get-server-time/)
- [AdsGetVersion]({{ site.baseurl }}/pt/funcoes/ads-get-version/)
- [AdsGetConnectionType]({{ site.baseurl }}/pt/funcoes/ads-get-connection-type/)

---

[AdsGetServerTime →]({{ site.baseurl }}/pt/funcoes/ads-get-server-time/)
