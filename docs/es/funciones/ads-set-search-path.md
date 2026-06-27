---
title: AdsSetSearchPath
layout: default
parent: Referencia API
nav_order: 1
permalink: /es/funciones/ads-set-search-path/
---

# AdsSetSearchPath

Establece la ruta de búsqueda de tablas registrada para la sesión.

## Sintaxis

```c
UNSIGNED32 AdsSetSearchPath(UNSIGNED8 *pucPath);
```

## Parámetros

| Parámetro | Tipo | Descripción |
|-----------|------|-------------|
| `pucPath` | `UNSIGNED8*` | Lista de directorios separados por punto y coma. Una cadena nula o vacía la borra. |

## Valor de Retorno

`AE_SUCCESS` (0).

## Descripción

`AdsSetSearchPath` registra una ruta de búsqueda que `AdsGetSearchPath` devuelve. En ADS es la lista de directorios consultados al abrir una tabla por nombre simple. OpenADS resuelve las rutas contra la ruta de datos de la conexión, por lo que el valor se almacena por paridad con la API ACE y hace round-trip con `AdsGetSearchPath`.

## Ejemplo

```c
AdsSetSearchPath((UNSIGNED8 *)"C:\\DATA;C:\\SHARED");
```

## Ver También

- [AdsGetSearchPath]({{ site.baseurl }}/es/funciones/ads-get-search-path/)
- [AdsSetDefault]({{ site.baseurl }}/es/funciones/ads-set-default/)

---

[AdsGetSearchPath →]({{ site.baseurl }}/es/funciones/ads-get-search-path/)
