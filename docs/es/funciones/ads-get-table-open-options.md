---
title: AdsGetTableOpenOptions
layout: default
parent: Referencia API
nav_order: 1
permalink: /es/funciones/ads-get-table-open-options/
---

# AdsGetTableOpenOptions

Devuelve las banderas de modo de apertura de una tabla.

## Sintaxis

```c
UNSIGNED32 AdsGetTableOpenOptions(ADSHANDLE hTable, UNSIGNED32 *pulOptions);
```

## Parámetros

| Parámetro | Tipo | Descripción |
|-----------|------|-------------|
| `hTable` | `ADSHANDLE` | Handle de la tabla. |
| `pulOptions` | `UNSIGNED32*` | Salida — máscara de bits de las banderas de modo de apertura. |

## Valor de Retorno

`AE_SUCCESS` (0) en caso de éxito.

## Descripción

`AdsGetTableOpenOptions` devuelve la máscara de bits de banderas
utilizadas cuando se abrió la tabla. Estas banderas incluyen
acceso de lectura/escritura, modo de compartición y otras opciones
a nivel de tabla.

## Ejemplo

```c
unsigned long options = 0;
AdsGetTableOpenOptions(hTable, &options);
printf("Opciones de apertura: 0x%08lX\n", options);
```

## Ver También

- [AdsOpenTable]({{ site.baseurl }}/es/funciones/ads-open-table/)
- [AdsGetTableType]({{ site.baseurl }}/es/funciones/ads-get-table-type/)
- [AdsGetTableCharType]({{ site.baseurl }}/es/funciones/ads-get-table-char-type/)

---

[← AdsGetTableFilename]({{ site.baseurl }}/es/funciones/ads-get-table-filename/)
[AdsGetTableConnection →]({{ site.baseurl }}/es/funciones/ads-get-table-connection/)
