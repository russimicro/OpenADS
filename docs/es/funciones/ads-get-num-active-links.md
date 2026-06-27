---
title: AdsGetNumActiveLinks
layout: default
parent: Referencia API
nav_order: 1
permalink: /es/funciones/ads-get-num-active-links/
---

# AdsGetNumActiveLinks

Devuelve el conteo de enlaces activos del diccionario de datos.

## Sintaxis

```c
UNSIGNED32 AdsGetNumActiveLinks(ADSHANDLE hTable, UNSIGNED16 *pusCount);
```

## Parámetros

| Parámetro | Tipo | Descripción |
|-----------|------|-------------|
| `hTable` | `ADSHANDLE` | Handle de la tabla. |
| `pusCount` | `UNSIGNED16*` | Salida — número de enlaces activos. |

## Valor de Retorno

`AE_SUCCESS` (0) en caso de éxito.

## Descripción

`AdsGetNumActiveLinks` devuelve el número de enlaces activos del
diccionario de datos asociados con la tabla dada. Los enlaces del
diccionario de datos definen relaciones entre tablas y se utilizan
para la integridad referencial y la optimización de consultas.

## Ejemplo

```c
unsigned short numLinks = 0;
AdsGetNumActiveLinks(hTable, &numLinks);
printf("Enlaces DD activos: %u\n", numLinks);
```

## Ver También

- [AdsGetNumOpenTables]({{ site.baseurl }}/es/funciones/ads-get-num-open-tables/)
- [AdsGetNumLocks]({{ site.baseurl }}/es/funciones/ads-get-num-locks/)
- [AdsGetTableConnection]({{ site.baseurl }}/es/funciones/ads-get-table-connection/)

---

[← AdsGetNumOpenTables]({{ site.baseurl }}/es/funciones/ads-get-num-open-tables/)
[AdsOpenTable →]({{ site.baseurl }}/es/funciones/ads-open-table/)
