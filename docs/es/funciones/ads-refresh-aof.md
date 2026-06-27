---
title: AdsRefreshAOF
layout: default
parent: Referencia API
nav_order: 1
permalink: /es/funciones/ads-refresh-aof/
---

# AdsRefreshAOF

Reevalúa el Advantage Optimized Filter activo contra los datos actuales.

## Sintaxis

```c
UNSIGNED32 AdsRefreshAOF(ADSHANDLE hTable);
```

## Parámetros

| Parámetro | Tipo | Descripción |
|-----------|------|-------------|
| `hTable` | `ADSHANDLE` | Handle de la tabla con un AOF activo. |

## Valor de Retorno

`AE_SUCCESS` (0) en caso de éxito, incluido cuando no hay AOF activo (no-op). `AE_INTERNAL_ERROR` (5000) si el handle es desconocido.

## Descripción

`AdsRefreshAOF` reevalúa la expresión AOF instalada con `AdsSetAOF` contra el contenido actual de la tabla y reinstala el bitmap resultante. Los registros añadidos o cuyos campos de clave cambiaron desde que se fijó el filtro se reclasifican, de modo que el conjunto visible refleja los datos más recientes.

Si no hay AOF activo, o el conjunto se construyó solo con `AdsCustomizeAOF` (sin expresión almacenada), la llamada tiene éxito sin cambiar nada. Para tablas remotas el servidor mantiene el AOF, por lo que la llamada es un no-op.

## Ejemplo

```c
AdsSetAOF(hTable, "BALANCE > 0", 0);
// ... los registros cambian o se añaden ...
AdsRefreshAOF(hTable);   // el conjunto visible refleja los nuevos datos
```

## Ver También

- [AdsSetAOF]({{ site.baseurl }}/es/funciones/ads-set-aof/)
- [AdsCustomizeAOF]({{ site.baseurl }}/es/funciones/ads-customize-aof/)
- [AdsClearAOF]({{ site.baseurl }}/es/funciones/ads-clear-aof/)

---

[AdsCustomizeAOF →]({{ site.baseurl }}/es/funciones/ads-customize-aof/)
