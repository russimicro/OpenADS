---
title: AdsTestRecLocks
layout: default
parent: Referencia API
nav_order: 1
permalink: /es/funciones/ads-test-rec-locks/
---

# AdsTestRecLocks

Hook de diagnóstico para los bloqueos de registro de una tabla.

## Sintaxis

```c
UNSIGNED32 AdsTestRecLocks(ADSHANDLE hTable);
```

## Parámetros

| Parámetro | Tipo | Descripción |
|-----------|------|-------------|
| `hTable` | `ADSHANDLE` | Handle de la tabla. |

## Valor de Retorno

`AE_SUCCESS` (0) en caso de éxito. `AE_INTERNAL_ERROR` (5000) si el handle es desconocido.

## Descripción

`AdsTestRecLocks` es un hook de diagnóstico. OpenADS no tiene una verificación de consistencia de la tabla de bloqueos separada que ejecutar, por lo que la llamada valida el handle de la tabla e informa éxito. Se ofrece por compatibilidad con la API ACE. Para inspeccionar el estado real de bloqueos, use `AdsGetAllLocks` o `AdsIsRecordLocked`.

## Ejemplo

```c
AdsTestRecLocks(hTable);
```

## Ver También

- [AdsGetAllLocks]({{ site.baseurl }}/es/funciones/ads-get-all-locks/)
- [AdsIsRecordLocked]({{ site.baseurl }}/es/funciones/ads-is-record-locked/)
- [AdsGetNumLocks]({{ site.baseurl }}/es/funciones/ads-get-num-locks/)

---

[AdsGetNumLocks →]({{ site.baseurl }}/es/funciones/ads-get-num-locks/)
