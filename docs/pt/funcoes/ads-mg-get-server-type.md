---
title: AdsMgGetServerType
layout: default
parent: Referência da API
nav_order: 24
permalink: /pt/funcoes/ads-mg-get-server-type/
---

# AdsMgGetServerType

Retorna o tipo do servidor Advantage.

## Sintaxe

```c
UNSIGNED32 AdsMgGetServerType(ADSHANDLE hMg, UNSIGNED16* pusT);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hMg` | `ADSHANDLE` | Handle da conexão de gerenciamento. |
| `pusT` | `UNSIGNED16*` | Ponteiro para variável que recebe o tipo do servidor. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsMgGetServerType` retorna o tipo do servidor Advantage ao qual a conexão de gerenciamento está conectado. Os valores retornados são:

- `ADS_AIS_SERVER` (1) — Servidor Advantage Internet Server
- `ADS_LINUX` (2) — Servidor Advantage no Linux

## Exemplo

```c
UNSIGNED16 usType;
AdsMgGetServerType(hMgmt, &usType);
if (usType == ADS_AIS_SERVER) {
    // Servidor AIS (Windows)
}
```

## Ver Também

- [AdsMgConnect]({{ site.baseurl }}/pt/funcoes/ads-mg-connect/)
- [AdsGetConnectionType]({{ site.baseurl }}/pt/funcoes/ads-get-connection-type/)
- [AdsGetServerName]({{ site.baseurl }}/pt/funcoes/ads-get-server-name/)

---

[AdsMgGetUserNames →]({{ site.baseurl }}/pt/funcoes/ads-mg-get-user-names/)
