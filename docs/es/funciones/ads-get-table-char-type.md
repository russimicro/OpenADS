---
title: AdsGetTableCharType
layout: default
parent: Referencia API
nav_order: 1
permalink: /es/funciones/ads-get-table-char-type/
---

# AdsGetTableCharType

Devuelve el tipo de carácter de una tabla (`ADS_ANSI` o `ADS_OEM`).

## Sintaxis

```c
UNSIGNED32 AdsGetTableCharType(ADSHANDLE hTable, UNSIGNED16 *pusType);
```

## Parámetros

| Parámetro | Tipo | Descripción |
|-----------|------|-------------|
| `hTable` | `ADSHANDLE` | Handle de la tabla. |
| `pusType` | `UNSIGNED16*` | Salida — constante del tipo de carácter. |

## Valor de Retorno

`AE_SUCCESS` (0) en caso de éxito.

## Constantes de Tipo de Carácter

| Constante | Valor | Descripción |
|-----------|-------|-------------|
| `ADS_OEM` | 0 | Juego de caracteres OEM (compatibilidad binaria). |
| `ADS_ANSI` | 1 | Juego de caracteres ANSI (Windows). |

## Descripción

`AdsGetTableCharType` devuelve si la tabla fue abierta con
traducción de caracteres OEM o ANSI. Esto afecta cómo se almacenan
y comparan los campos de caracteres.

## Ejemplo

```c
UNSIGNED16 charType = 0;
AdsGetTableCharType(hTable, &charType);
if (charType == ADS_ANSI)
    printf("La tabla usa caracteres ANSI\n");
else
    printf("La tabla usa caracteres OEM\n");
```

## Ver También

- [AdsOpenTable]({{ site.baseurl }}/es/funciones/ads-open-table/)
- [AdsGetTableType]({{ site.baseurl }}/es/funciones/ads-get-table-type/)
- [AdsGetTableAlias]({{ site.baseurl }}/es/funciones/ads-get-table-alias/)

---

[← AdsGetTableConType]({{ site.baseurl }}/es/funciones/ads-get-table-con-type/)
[AdsGetTableAlias →]({{ site.baseurl }}/es/funciones/ads-get-table-alias/)
