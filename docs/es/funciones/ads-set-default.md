---
title: AdsSetDefault
layout: default
parent: Referencia API
nav_order: 1
permalink: /es/funciones/ads-set-default/
---

# AdsSetDefault

Establece el directorio por defecto registrado para la sesión.

## Sintaxis

```c
UNSIGNED32 AdsSetDefault(UNSIGNED8 *pucPath);
```

## Parámetros

| Parámetro | Tipo | Descripción |
|-----------|------|-------------|
| `pucPath` | `UNSIGNED8*` | Ruta del directorio por defecto. Una cadena nula o vacía lo borra. |

## Valor de Retorno

`AE_SUCCESS` (0).

## Descripción

`AdsSetDefault` registra una cadena de directorio por defecto que `AdsGetDefault` devuelve. En ADS es el directorio usado para resolver nombres de tabla relativos. OpenADS resuelve las rutas contra la ruta de datos de la conexión, por lo que el valor se almacena por paridad con la API ACE y hace round-trip con `AdsGetDefault`.

## Ejemplo

```c
AdsSetDefault((UNSIGNED8 *)"C:\\DATA\\APP");

UNSIGNED8 buf[260];
UNSIGNED16 len = sizeof(buf);
AdsGetDefault(buf, &len);   // "C:\DATA\APP"
```

## Ver También

- [AdsGetDefault]({{ site.baseurl }}/es/funciones/ads-get-default/)
- [AdsSetSearchPath]({{ site.baseurl }}/es/funciones/ads-set-search-path/)

---

[AdsGetDefault →]({{ site.baseurl }}/es/funciones/ads-get-default/)
