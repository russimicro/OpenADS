---
title: AdsSetRecord
layout: default
parent: Referencia API
nav_order: 1
permalink: /es/funciones/ads-set-record/
---

# AdsSetRecord

Sobrescribe el registro actual con una imagen física en bruto.

## Sintaxis

```c
UNSIGNED32 AdsSetRecord(ADSHANDLE hTable, UNSIGNED8 *pucRecord, UNSIGNED32 ulLen);
```

## Parámetros

| Parámetro | Tipo | Descripción |
|-----------|------|-------------|
| `hTable` | `ADSHANDLE` | Handle de la tabla. |
| `pucRecord` | `UNSIGNED8*` | Buffer con la imagen en bruto del registro (byte de borrado + bytes de campos). |
| `ulLen` | `UNSIGNED32` | Longitud de la imagen suministrada, en bytes. |

## Valor de Retorno

`AE_SUCCESS` (0) en caso de éxito. `AE_NO_CURRENT_RECORD` (5068) cuando el cursor está en BOF/EOF. `AE_INTERNAL_ERROR` (5000) si el buffer es nulo, la tabla es de solo lectura o el handle es desconocido.

## Descripción

`AdsSetRecord` escribe una imagen física completa del registro —tal como la produce `AdsGetRecord`— sobre el registro actual, lo vuelca a disco y resincroniza todos los índices vinculados, de modo que cualquier cambio en un campo de clave se refleja en el orden del índice.

Se copian como mucho *longitud del registro* bytes; si `ulLen` es menor, solo se escriben esos bytes y el resto del registro queda intacto. El primer byte de la imagen es la marca de borrado (un espacio si está activo, `*` si está borrado).

Es la contraparte de escritura de `AdsGetRecord`. No está disponible para tablas remotas.

## Ejemplo

```c
UNSIGNED32 ulLen = 0;

AdsGetRecord(hTable, NULL, &ulLen);
UNSIGNED8 *pucRec = malloc(ulLen);
AdsGetRecord(hTable, pucRec, &ulLen);

// Parchear un campo de ancho fijo y reescribir.
memcpy(pucRec + 5, "BBBBBBBB", 8);
AdsSetRecord(hTable, pucRec, ulLen);

free(pucRec);
```

## Ver También

- [AdsGetRecord]({{ site.baseurl }}/es/funciones/ads-get-record/)
- [AdsWriteRecord]({{ site.baseurl }}/es/funciones/ads-write-record/)
- [AdsAppendRecord]({{ site.baseurl }}/es/funciones/ads-append-record/)
- [AdsGetRecordLength]({{ site.baseurl }}/es/funciones/ads-get-record-length/)

---

[← AdsGetRecord]({{ site.baseurl }}/es/funciones/ads-get-record/)
