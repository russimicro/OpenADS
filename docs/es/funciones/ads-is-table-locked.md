---
title: AdsIsTableLocked
layout: default
parent: Referencia API
nav_order: 1
permalink: /es/funciones/ads-is-table-locked/
---

# AdsIsTableLocked

Verifica si una tabla tiene bloqueo exclusivo.

## Sintaxis

```c
UNSIGNED32 AdsIsTableLocked(ADSHANDLE hTable, UNSIGNED16 *pbLocked);
```

## Parámetros

| Parámetro | Tipo | Descripción |
|-----------|------|-------------|
| `hTable` | `ADSHANDLE` | Handle de la tabla. |
| `pbLocked` | `UNSIGNED16*` | Salida — `ADS_TRUE` si la tabla tiene bloqueo exclusivo, `ADS_FALSE` en caso contrario. |

## Valor de Retorno

`AE_SUCCESS` (0) en caso de éxito.

## Descripción

`AdsIsTableLocked` verifica si la tabla especificada tiene un
bloqueo exclusivo mantenido. Un bloqueo exclusivo impide que otros
usuarios accedan a la tabla. Esto es diferente de los bloqueos de
registro individuales, que solo bloquean registros específicos.

## Ejemplo

```c
unsigned short bLocked = 0;
AdsIsTableLocked(hTable, &bLocked);
if (bLocked == ADS_TRUE)
    printf("La tabla tiene bloqueo exclusivo\n");
else
    printf("La tabla no tiene bloqueo exclusivo\n");
```

## Ver También

- [AdsLockTable]({{ site.baseurl }}/es/funciones/ads-lock-table/)
- [AdsUnlockTable]({{ site.baseurl }}/es/funciones/ads-unlock-table/)
- [AdsGetNumLocks]({{ site.baseurl }}/es/funciones/ads-get-num-locks/)

---

[← AdsIsRecordLocked]({{ site.baseurl }}/es/funciones/ads-is-record-locked/)
[AdsLockRecord →]({{ site.baseurl }}/es/funciones/ads-lock-record/)
