---
title: AdsCacheRecords
layout: default
parent: Referencia API
nav_order: 1
permalink: /es/funciones/ads-cache-records/
---

# AdsCacheRecords

Sugiere cuántos registros leer por adelantado para una tabla.

## Sintaxis

```c
UNSIGNED32 AdsCacheRecords(ADSHANDLE hTable, UNSIGNED16 usNumRecords);
```

## Parámetros

| Parámetro | Tipo | Descripción |
|-----------|------|-------------|
| `hTable` | `ADSHANDLE` | Handle de la tabla. |
| `usNumRecords` | `UNSIGNED16` | Número sugerido de registros a leer por adelantado. |

## Valor de Retorno

`AE_SUCCESS` (0) en caso de éxito. `AE_INTERNAL_ERROR` (5000) si el handle es desconocido.

## Descripción

`AdsCacheRecords` es una sugerencia de lectura anticipada. OpenADS no precarga filas, por lo que la llamada valida el handle de la tabla y tiene éxito sin cambiar el comportamiento. Se ofrece por compatibilidad con la API ACE para código que ajusta la caché del cliente.

## Ejemplo

```c
AdsCacheRecords(hTable, 50);
```

## Ver También

- [AdsCacheOpenTables]({{ site.baseurl }}/es/funciones/ads-cache-open-tables/)
- [AdsCacheOpenCursors]({{ site.baseurl }}/es/funciones/ads-cache-open-cursors/)

---

[AdsCacheOpenTables →]({{ site.baseurl }}/es/funciones/ads-cache-open-tables/)
