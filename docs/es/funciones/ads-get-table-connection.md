---
title: AdsGetTableConnection
layout: default
parent: Referencia API
nav_order: 1
permalink: /es/funciones/ads-get-table-connection/
---

# AdsGetTableConnection

Devuelve el handle de conexión asociado con una tabla.

## Sintaxis

```c
UNSIGNED32 AdsGetTableConnection(ADSHANDLE hTable, ADSHANDLE *phConnect);
```

## Parámetros

| Parámetro | Tipo | Descripción |
|-----------|------|-------------|
| `hTable` | `ADSHANDLE` | Handle de la tabla. |
| `phConnect` | `ADSHANDLE*` | Salida — handle de conexión de la tabla. |

## Valor de Retorno

`AE_SUCCESS` (0) en caso de éxito.

## Descripción

`AdsGetTableConnection` recupera el handle de conexión que se
utilizó para abrir la tabla dada. Para tablas locales, esta es la
conexión del motor local integrada. Para tablas remotas, es la
conexión TCP/TLS con el servidor.

## Ejemplo

```c
ADSHANDLE hConn = 0;
AdsGetTableConnection(hTable, &hConn);
printf("La tabla está en la conexión: %p\n", (void *)hConn);
```

## Ver También

- [AdsConnect60]({{ site.baseurl }}/es/funciones/ads-connect60/)
- [AdsGetConnectionType]({{ site.baseurl }}/es/funciones/ads-get-connection-type/)
- [AdsIsConnectionAlive]({{ site.baseurl }}/es/funciones/ads-is-connection-alive/)

---

[← AdsGetTableAlias]({{ site.baseurl }}/es/funciones/ads-get-table-alias/)
[AdsGetTableOpenOptions →]({{ site.baseurl }}/es/funciones/ads-get-table-open-options/)
