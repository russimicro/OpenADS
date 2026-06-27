---
title: AdsGetFilter
layout: default
parent: Referencia API
nav_order: 1
permalink: /es/funciones/ads-get-filter/
---

# AdsGetFilter

Devuelve la expresión de filtro actual de una tabla.

## Sintaxis

```c
UNSIGNED32 AdsGetFilter(ADSHANDLE hTable, UNSIGNED8 *pucBuf, UNSIGNED16 *pusLen);
```

## Parámetros

| Parámetro | Tipo | Descripción |
|-----------|------|-------------|
| `hTable` | `ADSHANDLE` | Handle de la tabla. |
| `pucBuf` | `UNSIGNED8*` | Buffer de salida para la cadena de la expresión de filtro. |
| `pusLen` | `UNSIGNED16*` | Entrada/salida — tamaño del buffer; recibe la longitud de la expresión devuelta. |

## Valor de Retorno

`AE_SUCCESS` (0) en caso de éxito.

## Descripción

`AdsGetFilter` recupera la cadena de expresión de filtro actual
instalada en la tabla mediante `AdsSetFilter`. Si no hay filtro
activo, la función devuelve una cadena vacía.

Nota: Esto devuelve la expresión de filtro no indexada. Para filtros
optimizados con Rushmore, use `AdsGetAOF` en su lugar.

## Ejemplo

```c
char buf[256];
unsigned short len = sizeof(buf);
AdsGetFilter(hTable, (unsigned char *)buf, &len);
if (len > 0)
    printf("Filtro: %s\n", buf);
else
    printf("No hay filtro activo\n");
```

## Ver También

- [AdsSetFilter]({{ site.baseurl }}/es/funciones/ads-set-filter/)
- [AdsClearFilter]({{ site.baseurl }}/es/funciones/ads-clear-filter/)
- [AdsGetAOF]({{ site.baseurl }}/es/funciones/ads-get-aof/)

---

[← AdsGetExact]({{ site.baseurl }}/es/funciones/ads-get-exact/)
[AdsGetHandleType →]({{ site.baseurl }}/es/funciones/ads-get-handle-type/)
