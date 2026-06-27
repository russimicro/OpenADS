---
title: AdsIsConnectionAlive
layout: default
parent: Referencia API
nav_order: 1
permalink: /es/funciones/ads-is-connection-alive/
---

# AdsIsConnectionAlive

Verifica si una conexión sigue activa (ping de heartbeat).

## Sintaxis

```c
UNSIGNED32 AdsIsConnectionAlive(ADSHANDLE hConnect, UNSIGNED16 *pbAlive);
```

## Parámetros

| Parámetro | Tipo | Descripción |
|-----------|------|-------------|
| `hConnect` | `ADSHANDLE` | Handle de la conexión. |
| `pbAlive` | `UNSIGNED16*` | Salida — `ADS_TRUE` si la conexión está activa, `ADS_FALSE` en caso contrario. |

## Valor de Retorno

`AE_SUCCESS` (0) en caso de éxito.

## Descripción

`AdsIsConnectionAlive` envía un ping de heartbeat al servidor para
verificar que la conexión sigue activa. Para conexiones locales,
siempre devuelve `ADS_TRUE`. Para conexiones remotas, realiza un
viaje de ida y vuelta de red para confirmar que el servidor es
alcanzable.

## Ejemplo

```c
ADSHANDLE hConn = 0;
AdsConnect60("tcp://server:6247", NULL, NULL, NULL, 0, &hConn);
unsigned short bAlive = 0;
AdsIsConnectionAlive(hConn, &bAlive);
if (bAlive == ADS_TRUE)
    printf("La conexión está activa\n");
else
    printf("La conexión está caída\n");
AdsDisconnect(hConn);
```

## Ver También

- [AdsConnect60]({{ site.baseurl }}/es/funciones/ads-connect60/)
- [AdsGetConnectionType]({{ site.baseurl }}/es/funciones/ads-get-connection-type/)
- [AdsDisconnect]({{ site.baseurl }}/es/funciones/ads-disconnect/)

---

[← AdsIsFound]({{ site.baseurl }}/es/funciones/ads-is-found/)
[AdsIsNull →]({{ site.baseurl }}/es/funciones/ads-is-null/)
