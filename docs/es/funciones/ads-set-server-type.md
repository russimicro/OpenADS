---
title: AdsSetServerType
layout: default
parent: Referencia API
nav_order: 1
permalink: /es/funciones/ads-set-server-type/
---

# AdsSetServerType

Registra el/los tipo(s) de servidor preferido(s) para las conexiones posteriores.

## Sintaxis

```c
UNSIGNED32 AdsSetServerType(UNSIGNED16 usServerOptions);
```

## Parámetros

| Parámetro | Tipo | Descripción |
|-----------|------|-------------|
| `usServerOptions` | `UNSIGNED16` | Máscara de tipos de servidor: `ADS_LOCAL_SERVER` (1), `ADS_REMOTE_SERVER` (2), `ADS_AIS_SERVER`. Combínelos con OR bit a bit. |

## Valor de Retorno

`AE_SUCCESS` (0).

## Descripción

`AdsSetServerType` registra qué tipo(s) de servidor prefiere la aplicación al establecer una conexión. En ADS esto restringe qué back-ends de servidor intentará `AdsConnect`.

OpenADS atiende conexiones locales y remotas con independencia de este ajuste, por lo que el valor se almacena por paridad con la API ACE y no bloquea una conexión por lo demás válida. Llámelo antes de `AdsConnect` si su código espera la semántica de ADS; pasar una máscara combinada (local + remoto) siempre es seguro.

## Ejemplo

```c
// Preferir un servidor remoto, con repliegue a local.
AdsSetServerType(ADS_REMOTE_SERVER | ADS_LOCAL_SERVER);

ADSHANDLE hConn;
AdsConnect((UNSIGNED8 *)"\\\\servidor\\recurso\\datos", &hConn);
```

## Ver También

- [AdsConnect]({{ site.baseurl }}/es/funciones/ads-connect/)
- [AdsGetConnectionType]({{ site.baseurl }}/es/funciones/ads-get-connection-type/)

---

[AdsConnect →]({{ site.baseurl }}/es/funciones/ads-connect/)
