---
title: AdsGetNumLocks
layout: default
parent: Referencia API
nav_order: 1
permalink: /es/funciones/ads-get-num-locks/
---

# AdsGetNumLocks

Devuelve el conteo de bloqueos de registro mantenidos en una tabla.

## Sintaxis

```c
UNSIGNED32 AdsGetNumLocks(ADSHANDLE hTable, UNSIGNED16 *pusCount);
```

## Parámetros

| Parámetro | Tipo | Descripción |
|-----------|------|-------------|
| `hTable` | `ADSHANDLE` | Handle de la tabla. |
| `pusCount` | `UNSIGNED16*` | Salida — número de bloqueos de registro activos. |

## Valor de Retorno

`AE_SUCCESS` (0) en caso de éxito.

## Descripción

`AdsGetNumLocks` devuelve cuántos bloqueos de registro se mantienen
actualmente en la tabla especificada. Este conteo incluye tanto
bloqueos explícitos (adquiridos mediante `AdsLockRecord`) como
bloqueos implícitos adquiridos durante la navegación o
actualizaciones.

## Ejemplo

```c
unsigned short numLocks = 0;
AdsGetNumLocks(hTable, &numLocks);
printf("Bloqueos de registro activos: %u\n", numLocks);
```

## Ver También

- [AdsLockRecord]({{ site.baseurl }}/es/funciones/ads-lock-record/)
- [AdsUnlockRecord]({{ site.baseurl }}/es/funciones/ads-unlock-record/)
- [AdsGetAllLocks]({{ site.baseurl }}/es/funciones/ads-get-all-locks/)

---

[← AdsGetKeyNum]({{ site.baseurl }}/es/funciones/ads-get-key-num/)
[AdsGetNumOpenTables →]({{ site.baseurl }}/es/funciones/ads-get-num-open-tables/)
