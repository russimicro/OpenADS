---
title: AdsShowError
layout: default
parent: Referencia API
nav_order: 1
permalink: /es/funciones/ads-show-error/
---

# AdsShowError

Informa de un mensaje de error al host.

## Sintaxis

```c
UNSIGNED32 AdsShowError(UNSIGNED8 *pucErrText);
```

## Parámetros

| Parámetro | Tipo | Descripción |
|-----------|------|-------------|
| `pucErrText` | `UNSIGNED8*` | Mensaje terminado en nulo a informar. Una cadena nula o vacía se ignora. |

## Valor de Retorno

`AE_SUCCESS` (0).

## Descripción

`AdsShowError` informa de un mensaje de error al host. En un host con interfaz gráfica ADS muestra un cuadro de mensaje; OpenADS es headless, por lo que el comportamiento fiel más cercano es escribir el mensaje en la salida de error estándar (`stderr`), seguido de un salto de línea. Un mensaje nulo o vacío no produce salida. La llamada siempre tiene éxito.

## Ejemplo

```c
UNSIGNED8 buf[256];
UNSIGNED16 len = sizeof(buf);
AdsGetLastError(&ulCode, buf, &len);
AdsShowError(buf);
```

## Ver También

- [AdsGetLastError]({{ site.baseurl }}/es/funciones/ads-get-last-error/)
- [AdsGetErrorString]({{ site.baseurl }}/es/funciones/ads-get-error-string/)

---

[AdsGetLastError →]({{ site.baseurl }}/es/funciones/ads-get-last-error/)
