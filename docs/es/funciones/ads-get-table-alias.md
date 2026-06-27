---
title: AdsGetTableAlias
layout: default
parent: Referencia API
nav_order: 1
permalink: /es/funciones/ads-get-table-alias/
---

# AdsGetTableAlias

Devuelve el alias de una tabla.

## Sintaxis

```c
UNSIGNED32 AdsGetTableAlias(ADSHANDLE hTable, UNSIGNED8 *pucBuf, UNSIGNED16 *pusLen);
```

## Parámetros

| Parámetro | Tipo | Descripción |
|-----------|------|-------------|
| `hTable` | `ADSHANDLE` | Handle de la tabla. |
| `pucBuf` | `UNSIGNED8*` | Buffer de salida para la cadena del alias. |
| `pusLen` | `UNSIGNED16*` | Entrada/salida — tamaño del buffer; recibe la longitud del alias devuelto. |

## Valor de Retorno

`AE_SUCCESS` (0) en caso de éxito.

## Descripción

`AdsGetTableAlias` recupera el nombre del alias asignado al handle
de tabla dado. El alias es el nombre utilizado en la sentencia
`USE` o cuando la tabla fue abierta.

## Ejemplo

```c
char alias[32];
unsigned short len = sizeof(alias);
AdsGetTableAlias(hTable, (unsigned char *)alias, &len);
printf("Alias: %s\n", alias);
```

## Ver También

- [AdsOpenTable]({{ site.baseurl }}/es/funciones/ads-open-table/)
- [AdsGetTableFilename]({{ site.baseurl }}/es/funciones/ads-get-table-filename/)
- [AdsGetTableType]({{ site.baseurl }}/es/funciones/ads-get-table-type/)

---

[← AdsGetTableCharType]({{ site.baseurl }}/es/funciones/ads-get-table-char-type/)
[AdsGetTableConnection →]({{ site.baseurl }}/es/funciones/ads-get-table-connection/)
