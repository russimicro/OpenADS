---
title: AdsGetDeleted
layout: default
parent: Referencia API
nav_order: 1
permalink: /es/funciones/ads-get-deleted/
---

# AdsGetDeleted

Devuelve si los registros eliminados están ocultos (estado `SET DELETED`).

## Sintaxis

```c
UNSIGNED32 AdsGetDeleted(UNSIGNED16 *pbShow);
```

## Parámetros

| Parámetro | Tipo | Descripción |
|-----------|------|-------------|
| `pbShow` | `UNSIGNED16*` | Salida — `ADS_TRUE` si los registros eliminados están ocultos, `ADS_FALSE` en caso contrario. |

## Valor de Retorno

`AE_SUCCESS` (0) en caso de éxito.

## Descripción

`AdsGetDeleted` consulta el estado actual de `SET DELETED` para el
proceso. Cuando está habilitado (`ADS_TRUE`), los registros
marcados como eliminados se excluyen de los comandos de navegación
como `Skip`, `GoTop` y `Seek`. Cuando está deshabilitado
(`ADS_FALSE`), los registros eliminados permanecen visibles.

Esto refleja el comportamiento de `SET DELETED` de Clipper y es
independiente de cualquier expresión AOF o filtro.

## Ejemplo

```c
UNSIGNED16 bShow = 0;
AdsGetDeleted(&bShow);
if (bShow == ADS_TRUE)
    printf("Los registros eliminados están ocultos\n");
else
    printf("Los registros eliminados son visibles\n");
```

## Ver También

- [AdsShowDeleted]({{ site.baseurl }}/es/funciones/ads-show-deleted/)
- [AdsGetFilter]({{ site.baseurl }}/es/funciones/ads-get-filter/)
- [AdsGetAOF]({{ site.baseurl }}/es/funciones/ads-get-aof/)

---

[← AdsGetDateFormat]({{ site.baseurl }}/es/funciones/ads-get-date-format/)
[AdsGetEpoch →]({{ site.baseurl }}/es/funciones/ads-get-epoch/)
