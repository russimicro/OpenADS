---
title: AdsGetEpoch
layout: default
parent: Referencia API
nav_order: 1
permalink: /es/funciones/ads-get-epoch/
---

# AdsGetEpoch

Devuelve el valor del pivot de año de 2 dígitos.

## Sintaxis

```c
UNSIGNED32 AdsGetEpoch(UNSIGNED16 *pusEpoch);
```

## Parámetros

| Parámetro | Tipo | Descripción |
|-----------|------|-------------|
| `pusEpoch` | `UNSIGNED16*` | Salida — el año pivot (por ejemplo, 1970 o 2000). |

## Valor de Retorno

`AE_SUCCESS` (0) en caso de éxito.

## Descripción

`AdsGetEpoch` devuelve el pivot de año de 2 dígitos a nivel de
proceso. Las fechas con años de 2 dígitos por debajo del pivot se
interpretan como fechas del siglo XXI; las que están en o por
encima se interpretan como fechas del siglo XX.

El epoch predeterminado es 1970. Esta configuración afecta cómo se
analizan las fechas cuando se almacenan como valores de año de 2
dígitos.

## Ejemplo

```c
UNSIGNED16 usEpoch = 0;
AdsGetEpoch(&usEpoch);
printf("Pivot de epoch: %u\n", usEpoch);
```

## Ver También

- [AdsSetEpoch]({{ site.baseurl }}/es/funciones/ads-set-epoch/)
- [AdsGetDateFormat]({{ site.baseurl }}/es/funciones/ads-get-date-format/)
- [AdsGetDeleted]({{ site.baseurl }}/es/funciones/ads-get-deleted/)

---

[← AdsGetDeleted]({{ site.baseurl }}/es/funciones/ads-get-deleted/)
[AdsGetExact →]({{ site.baseurl }}/es/funciones/ads-get-exact/)
