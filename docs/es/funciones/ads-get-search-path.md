---
title: AdsGetSearchPath
layout: default
parent: Referencia API
nav_order: 1
permalink: /es/funciones/ads-get-search-path/
---

# AdsGetSearchPath

Devuelve la ruta de búsqueda de tablas registrada para la sesión.

## Sintaxis

```c
UNSIGNED32 AdsGetSearchPath(UNSIGNED8 *pucPath, UNSIGNED16 *pusLen);
```

## Parámetros

| Parámetro | Tipo | Descripción |
|-----------|------|-------------|
| `pucPath` | `UNSIGNED8*` | Buffer que recibe la cadena de ruta de búsqueda. |
| `pusLen` | `UNSIGNED16*` | Entrada/salida — tamaño del buffer a la entrada; longitud de la cadena a la salida. |

## Valor de Retorno

`AE_SUCCESS` (0).

## Descripción

`AdsGetSearchPath` devuelve la ruta de búsqueda establecida previamente con `AdsSetSearchPath`, o una cadena vacía si no se estableció ninguna. Ambas funciones forman un round-trip.

## Ejemplo

```c
UNSIGNED8 buf[512];
UNSIGNED16 len = sizeof(buf);
AdsGetSearchPath(buf, &len);
```

## Ver También

- [AdsSetSearchPath]({{ site.baseurl }}/es/funciones/ads-set-search-path/)
- [AdsGetDefault]({{ site.baseurl }}/es/funciones/ads-get-default/)

---

[← AdsSetSearchPath]({{ site.baseurl }}/es/funciones/ads-set-search-path/)
