---
title: AdsGetConnectionType
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-connection-type/
---

# AdsGetConnectionType

Retorna o tipo de conexão.

## Sintaxe

```c
UNSIGNED32 AdsGetConnectionType(ADSHANDLE hConnect, UNSIGNED16* p);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `p` | `UNSIGNED16*` | Ponteiro para receber o tipo de conexão. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsGetConnectionType` retorna o tipo de conexão:
- `ADS_LOCAL_SERVER` (0) - Servidor local
- `ADS_REMOTE_SERVER` (1) - Servidor remoto

## Exemplo

```c
UNSIGNED16 usType;
AdsGetConnectionType(hConnect, &usType);
// usType é ADS_LOCAL_SERVER ou ADS_REMOTE_SERVER
```

## Ver Também

- [AdsConnect]({{ site.baseurl }}/pt/funcoes/ads-connect/)
- [AdsConnect60]({{ site.baseurl }}/pt/funcoes/ads-connect-60/)
- [AdsDisconnect]({{ site.baseurl }}/pt/funcoes/ads-disconnect/)

---

[AdsGetHandleType →]({{ site.baseurl }}/pt/funcoes/ads-get-handle-type/)
