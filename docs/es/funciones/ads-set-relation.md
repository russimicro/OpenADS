---
title: AdsSetRelation
layout: default
parent: Referencia API
nav_order: 1
permalink: /es/funciones/ads-set-relation/
---

# AdsSetRelation

Establece una relación padre-hijo entre áreas de trabajo para que el hijo siga al padre.

## Sintaxis

```c
UNSIGNED32 AdsSetRelation(ADSHANDLE hTableParent, ADSHANDLE hTableChild, UNSIGNED8 *pucExpr);
```

## Parámetros

| Parámetro | Tipo | Descripción |
|-----------|------|-------------|
| `hTableParent` | `ADSHANDLE` | Handle de la tabla padre (controladora). |
| `hTableChild` | `ADSHANDLE` | Handle de la tabla hija (relacionada). |
| `pucExpr` | `UNSIGNED8*` | Expresión de relación, evaluada sobre el registro actual del padre para producir la clave de búsqueda del hijo. |

## Valor de Retorno

`AE_SUCCESS` (0) en caso de éxito. `AE_FUNCTION_NOT_AVAILABLE` (5004) para tablas remotas. `AE_INTERNAL_ERROR` (5000) si la expresión es nula o el handle es desconocido.

## Descripción

`AdsSetRelation` vincula una tabla hija a una padre, de modo que cada vez que el cursor del padre se mueve, el hijo se reposiciona automáticamente. Tras cada navegación del padre (`AdsGotoTop`, `AdsGotoBottom`, `AdsSkip`, `AdsGotoRecord`, `AdsSeek`), la expresión de relación se evalúa sobre el registro actual del padre y el resultado se usa para buscar en el hijo:

- Si el hijo tiene un orden controlador, el valor se busca en ese índice (búsqueda blanda). Un fallo deja al hijo en EOF.
- Si el hijo no tiene orden controlador, el valor se trata como un número de registro y el hijo se mueve a él.

El hijo se posiciona de inmediato al establecer la relación, usando el registro actual del padre. Se pueden relacionar varios hijos con el mismo padre, y las relaciones se propagan en cadenas padre-hijo. Un padre puede establecer más de una relación; use `AdsClearRelation` para eliminarlas. Cerrar cualquiera de las tablas descarta las relaciones afectadas. No está disponible para tablas remotas.

## Ejemplo

```c
// Padre ORDERS, hijo CUSTOMERS indexado por su ID numérico.
AdsSetRelation(hOrders, hCustomers, "CUST_ID");

AdsGotoTop(hOrders);          // el hijo busca el cliente correspondiente
// ... leer campos de hCustomers para el pedido actual ...
AdsSkip(hOrders, 1);          // el hijo sigue automáticamente
```

## Ver También

- [AdsClearRelation]({{ site.baseurl }}/es/funciones/ads-clear-relation/)
- [AdsSeek]({{ site.baseurl }}/es/funciones/ads-seek/)
- [AdsSetIndexOrder]({{ site.baseurl }}/es/funciones/ads-set-index-order/)

---

[AdsClearRelation →]({{ site.baseurl }}/es/funciones/ads-clear-relation/)
