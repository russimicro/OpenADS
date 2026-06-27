---
title: AdsSetServerType
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-set-server-type/
---

# AdsSetServerType

Define o tipo de servidor preferido.

## Sintaxe

```c
UNSIGNED32 AdsSetServerType(UNSIGNED16 usServerOptions);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `usServerOptions` | `UNSIGNED16` | Máscara de tipo de servidor. |

## Valor de Retorno

`AE_SUCCESS` (0) sempre.

## Descrição

`AdsSetServerType` define o tipo de servidor preferido (ADS_LOCAL_SERVER / ADS_REMOTE_SERVER / ADS_AIS_SERVER). O OpenADS serve ambos os tipos independentemente desta configuração.

## Exemplo

```c
AdsSetServerType(ADS_LOCAL_SERVER);
```

## Ver Também

- [AdsGetConnectionType]({{ site.baseurl }}/pt/funcoes/ads-get-connection-type/)
- [AdsConnect]({{ site.baseurl }}/pt/funcoes/ads-connect/)
- [AdsConnect60]({{ site.baseurl }}/pt/funcoes/ads-connect-60/)

---

[AdsGetTableType →]({{ site.baseurl }}/pt/funcoes/ads-get-table-type/)
