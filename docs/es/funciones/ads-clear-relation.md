---
title: AdsClearRelation
layout: default
parent: Referencia API
nav_order: 1
permalink: /es/funciones/ads-clear-relation/
---

# AdsClearRelation

Elimina todas las relaciones establecidas desde una tabla padre.

## Sintaxis

```c
UNSIGNED32 AdsClearRelation(ADSHANDLE hTableParent);
```

## Parámetros

| Parámetro | Tipo | Descripción |
|-----------|------|-------------|
| `hTableParent` | `ADSHANDLE` | Handle de la tabla padre cuyas relaciones se eliminan. |

## Valor de Retorno

`AE_SUCCESS` (0) en caso de éxito.

## Descripción

`AdsClearRelation` elimina todas las relaciones padre-hijo establecidas previamente desde `hTableParent` con `AdsSetRelation`. Tras la llamada, mover el cursor del padre ya no reposiciona a los hijos antes relacionados; cada hijo mantiene su posición actual.

Solo se eliminan las relaciones que esta tabla controla **como padre**. Si la misma tabla es además hija de otra área de trabajo, ese vínculo permanece intacto. Las relaciones también se descartan automáticamente al cerrar la tabla padre o cualquier hija.

## Ejemplo

```c
AdsSetRelation(hOrders, hCustomers, "CUST_ID");
// ... navegar con la relación activa ...

AdsClearRelation(hOrders);    // el hijo ya no sigue al padre
```

## Ver También

- [AdsSetRelation]({{ site.baseurl }}/es/funciones/ads-set-relation/)
- [AdsClearScope]({{ site.baseurl }}/es/funciones/ads-clear-scope/)

---

[← AdsSetRelation]({{ site.baseurl }}/es/funciones/ads-set-relation/)
