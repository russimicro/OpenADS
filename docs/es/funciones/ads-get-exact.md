---
title: AdsGetExact
layout: default
parent: Referencia API
nav_order: 1
permalink: /es/funciones/ads-get-exact/
---

# AdsGetExact

Devuelve si `SET EXACT` está habilitado.

## Sintaxis

```c
UNSIGNED32 AdsGetExact(UNSIGNED16 *pbExact);
```

## Parámetros

| Parámetro | Tipo | Descripción |
|-----------|------|-------------|
| `pbExact` | `UNSIGNED16*` | Salida — `ADS_TRUE` si la comparación exacta está habilitada, `ADS_FALSE` en caso contrario. |

## Valor de Retorno

`AE_SUCCESS` (0) en caso de éxito.

## Descripción

`AdsGetExact` consulta el estado actual de `SET EXACT` para el
proceso. Cuando está habilitado, las comparaciones de cadenas
requieren que ambas cadenas coincidan exactamente en longitud y
contenido. Cuando está deshabilitado, una comparación de
`"ABC" = "AB"` devuelve true (los espacios/caracteres finales se
ignoran).

Esto refleja el comportamiento de `SET EXACT` de Clipper.

## Ejemplo

```c
UNSIGNED16 bExact = 0;
AdsGetExact(&bExact);
if (bExact == ADS_TRUE)
    printf("SET EXACT está activado\n");
else
    printf("SET EXACT está desactivado\n");
```

## Ver También

- [AdsSetExact]({{ site.baseurl }}/es/funciones/ads-set-exact/)
- [AdsGetDeleted]({{ site.baseurl }}/es/funciones/ads-get-deleted/)
- [AdsGetEpoch]({{ site.baseurl }}/es/funciones/ads-get-epoch/)

---

[← AdsGetEpoch]({{ site.baseurl }}/es/funciones/ads-get-epoch/)
[AdsGetFilter →]({{ site.baseurl }}/es/funciones/ads-get-filter/)
