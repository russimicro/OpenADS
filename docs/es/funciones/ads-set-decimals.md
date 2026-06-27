---
title: AdsSetDecimals
layout: default
parent: Referencia API
nav_order: 1
permalink: /es/funciones/ads-set-decimals/
---

# AdsSetDecimals

Establece el número de decimales por defecto registrado para la sesión.

## Sintaxis

```c
UNSIGNED32 AdsSetDecimals(UNSIGNED16 usDecimals);
```

## Parámetros

| Parámetro | Tipo | Descripción |
|-----------|------|-------------|
| `usDecimals` | `UNSIGNED16` | Número de decimales por defecto. |

## Valor de Retorno

`AE_SUCCESS` (0).

## Descripción

`AdsSetDecimals` registra el número de decimales por defecto (el ajuste SET DECIMALS). Se almacena por paridad con la API ACE; OpenADS lee y escribe los campos numéricos con la precisión definida en el esquema con independencia de este valor.

## Ejemplo

```c
AdsSetDecimals(4);
```

## Ver También

- [AdsSetExact]({{ site.baseurl }}/es/funciones/ads-set-exact/)
- [AdsSetEpoch]({{ site.baseurl }}/es/funciones/ads-set-epoch/)

---

[AdsSetExact →]({{ site.baseurl }}/es/funciones/ads-set-exact/)
