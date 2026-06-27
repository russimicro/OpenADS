---
title: AdsGetAOF
layout: default
parent: Referencia API
nav_order: 1
permalink: /es/funciones/ads-get-aof/
---

# AdsGetAOF

Devuelve la cadena de expresión del Advantage Optimized Filter (AOF) actual.

## Sintaxis

```c
UNSIGNED32 AdsGetAOF(ADSHANDLE hTable, UNSIGNED8 *pucFilter, UNSIGNED16 *pusLen);
```

## Parámetros

| Parámetro | Tipo | Descripción |
|-----------|------|-------------|
| `hTable` | `ADSHANDLE` | Handle de la tabla. |
| `pucFilter` | `UNSIGNED8*` | Buffer de salida para la cadena de expresión del AOF. |
| `pusLen` | `UNSIGNED16*` | Entrada/salida — tamaño del buffer; recibe la longitud de la expresión devuelta. |

## Valor de Retorno

`AE_SUCCESS` (0) en caso de éxito.

## Descripción

`AdsGetAOF` recupera la cadena de expresión del AOF actual
instalada en la tabla mediante `AdsSetAOF`. Si no hay AOF activo,
la función devuelve una cadena vacía.

Las expresiones AOF son predicados de filtro optimizados con
Rushmore que pueden aprovechar las claves de índice para el
filtrado rápido de registros. Use `AdsGetAOFOptLevel` para
verificar el nivel de optimización.

## Ejemplo

```c
char buf[256];
unsigned short len = sizeof(buf);
AdsGetAOF(hTable, (unsigned char *)buf, &len);
if (len > 0)
    printf("AOF: %s\n", buf);
else
    printf("No hay AOF activo\n");
```

## Ver También

- [AdsSetAOF]({{ site.baseurl }}/es/funciones/ads-set-aof/)
- [AdsClearAOF]({{ site.baseurl }}/es/funciones/ads-clear-aof/)
- [AdsGetAOFOptLevel]({{ site.baseurl }}/es/funciones/ads-get-aofopt-level/)

---

[← AdsEvalAOF]({{ site.baseurl }}/es/funciones/ads-eval-aof/)
[AdsCustomizeAOF →]({{ site.baseurl }}/es/funciones/ads-customize-aof/)
