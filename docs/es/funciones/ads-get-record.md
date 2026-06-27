---
title: AdsGetRecord
layout: default
parent: Referencia API
nav_order: 1
permalink: /es/funciones/ads-get-record/
---

# AdsGetRecord

Copia la imagen física en bruto del registro actual a un buffer del llamante.

## Sintaxis

```c
UNSIGNED32 AdsGetRecord(ADSHANDLE hTable, UNSIGNED8 *pucRecord, UNSIGNED32 *pulLen);
```

## Parámetros

| Parámetro | Tipo | Descripción |
|-----------|------|-------------|
| `hTable` | `ADSHANDLE` | Handle de la tabla. |
| `pucRecord` | `UNSIGNED8*` | Buffer del llamante que recibe la imagen en bruto del registro. Puede ser `NULL` para consultar el tamaño requerido. |
| `pulLen` | `UNSIGNED32*` | Entrada/salida — capacidad del buffer a la entrada; bytes copiados a la salida. |

## Valor de Retorno

`AE_SUCCESS` (0) en caso de éxito. `AE_INSUFFICIENT_BUFFER` (5051) si el buffer es menor que el registro; la longitud requerida se devuelve en `pulLen`. `AE_NO_CURRENT_RECORD` (5068) cuando el cursor está en BOF/EOF.

## Descripción

`AdsGetRecord` devuelve la imagen física completa del registro tal como se almacena en la tabla: el byte inicial de marca de borrado (un espacio si el registro está activo, `*` si está borrado) seguido de los bytes en bruto de los campos. Los datos se copian literalmente y **no** terminan en NUL, por lo que pueden contener bytes nulos o altos.

Si `pucRecord` es `NULL` o `*pulLen` es 0, la llamada se trata como una consulta de tamaño: la longitud del registro se escribe en `*pulLen` sin copiar bytes, de modo que el llamante puede reservar el buffer exacto y volver a llamar.

Es la función complementaria de `AdsSetRecord`, que escribe una imagen en bruto sobre el registro actual. No está disponible para tablas remotas.

## Ejemplo

```c
UNSIGNED32 ulLen = 0;

// Consulta de tamaño y lectura.
AdsGetRecord(hTable, NULL, &ulLen);
UNSIGNED8 *pucRec = malloc(ulLen);
AdsGetRecord(hTable, pucRec, &ulLen);

printf("borrado=%d, %u bytes\n", pucRec[0] == '*', ulLen);
free(pucRec);
```

## Ver También

- [AdsSetRecord]({{ site.baseurl }}/es/funciones/ads-set-record/)
- [AdsGetField]({{ site.baseurl }}/es/funciones/ads-get-field/)
- [AdsGetRecordLength]({{ site.baseurl }}/es/funciones/ads-get-record-length/)
- [AdsIsRecordDeleted]({{ site.baseurl }}/es/funciones/ads-is-record-deleted/)

---

[AdsSetRecord →]({{ site.baseurl }}/es/funciones/ads-set-record/)
