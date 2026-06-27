---
title: AdsGetRecordCRC
layout: default
parent: Referencia API
nav_order: 1
permalink: /es/funciones/ads-get-record-crc/
---

# AdsGetRecordCRC

Calcula una suma de verificación CRC de la imagen del registro actual.

## Sintaxis

```c
UNSIGNED32 AdsGetRecordCRC(ADSHANDLE hTable, UNSIGNED32 *pulCRC, UNSIGNED32 ulOptions);
```

## Parámetros

| Parámetro | Tipo | Descripción |
|-----------|------|-------------|
| `hTable` | `ADSHANDLE` | Handle de la tabla. |
| `pulCRC` | `UNSIGNED32*` | Recibe el CRC de 32 bits del registro actual. |
| `ulOptions` | `UNSIGNED32` | Reservado; pase 0. |

## Valor de Retorno

`AE_SUCCESS` (0) en caso de éxito. `AE_NO_CURRENT_RECORD` (5068) cuando el cursor está en BOF/EOF. `AE_FUNCTION_NOT_AVAILABLE` (5004) para tablas remotas.

## Descripción

`AdsGetRecordCRC` devuelve una suma de verificación de 32 bits calculada sobre la imagen física en bruto del registro —los mismos bytes que devuelve `AdsGetRecord`, incluido el byte inicial de marca de borrado. Usa el CRC-32 IEEE estándar (reflejado, polinomio `0xEDB88320`).

La suma es una forma rápida de detectar si un registro cambió: leer el CRC, hacer otro trabajo, leerlo de nuevo y comparar. Dos registros con bytes de campo idénticos producen el mismo CRC; cualquier diferencia en la imagen da un valor distinto. El valor es estable para una imagen de registro dada, pero **no** se garantiza que coincida con el de otra implementación de ADS.

No está disponible para tablas remotas.

## Ejemplo

```c
UNSIGNED32 ulAntes = 0, ulDespues = 0;

AdsGetRecordCRC(hTable, &ulAntes, 0);
// ... otro proceso podría actualizar la fila ...
AdsRefreshRecord(hTable);
AdsGetRecordCRC(hTable, &ulDespues, 0);

if (ulAntes != ulDespues)
    printf("registro cambiado\n");
```

## Ver También

- [AdsGetRecord]({{ site.baseurl }}/es/funciones/ads-get-record/)
- [AdsRefreshRecord]({{ site.baseurl }}/es/funciones/ads-refresh-record/)
- [AdsGetRecordLength]({{ site.baseurl }}/es/funciones/ads-get-record-length/)

---

[AdsGetRecord →]({{ site.baseurl }}/es/funciones/ads-get-record/)
