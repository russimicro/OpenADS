---
title: AdsGetDefault
layout: default
parent: Referencia API
nav_order: 1
permalink: /es/funciones/ads-get-default/
---

# AdsGetDefault

Devuelve el directorio por defecto registrado para la sesión.

## Sintaxis

```c
UNSIGNED32 AdsGetDefault(UNSIGNED8 *pucPath, UNSIGNED16 *pusLen);
```

## Parámetros

| Parámetro | Tipo | Descripción |
|-----------|------|-------------|
| `pucPath` | `UNSIGNED8*` | Buffer que recibe la cadena del directorio por defecto. |
| `pusLen` | `UNSIGNED16*` | Entrada/salida — tamaño del buffer a la entrada; longitud de la cadena a la salida. |

## Valor de Retorno

`AE_SUCCESS` (0).

## Descripción

`AdsGetDefault` devuelve la cadena de directorio establecida previamente con `AdsSetDefault`, o una cadena vacía si no se estableció ninguna. Ambas funciones forman un round-trip; vea `AdsSetDefault` para cómo trata OpenADS el valor.

## Ejemplo

```c
UNSIGNED8 buf[260];
UNSIGNED16 len = sizeof(buf);
AdsGetDefault(buf, &len);
```

## Ver También

- [AdsSetDefault]({{ site.baseurl }}/es/funciones/ads-set-default/)
- [AdsGetSearchPath]({{ site.baseurl }}/es/funciones/ads-get-search-path/)

---

[← AdsSetDefault]({{ site.baseurl }}/es/funciones/ads-set-default/)
