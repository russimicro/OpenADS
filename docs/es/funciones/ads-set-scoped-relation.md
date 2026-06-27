---
title: AdsSetScopedRelation
layout: default
parent: Referencia API
nav_order: 1
permalink: /es/funciones/ads-set-scoped-relation/
---

# AdsSetScopedRelation

Establece una relación padre-hijo que además acota el hijo al grupo de clave coincidente.

## Sintaxis

```c
UNSIGNED32 AdsSetScopedRelation(ADSHANDLE hTableParent, ADSHANDLE hTableChild, UNSIGNED8 *pucExpr);
```

## Parámetros

| Parámetro | Tipo | Descripción |
|-----------|------|-------------|
| `hTableParent` | `ADSHANDLE` | Handle de la tabla padre (controladora). |
| `hTableChild` | `ADSHANDLE` | Handle de la tabla hija (relacionada). |
| `pucExpr` | `UNSIGNED8*` | Expresión de relación, evaluada sobre el registro actual del padre para producir la clave del hijo. |

## Valor de Retorno

`AE_SUCCESS` (0) en caso de éxito. `AE_FUNCTION_NOT_AVAILABLE` (5004) para tablas remotas. `AE_INTERNAL_ERROR` (5000) si la expresión es nula o el handle es desconocido.

## Descripción

`AdsSetScopedRelation` funciona igual que `AdsSetRelation` —al mover el padre, el hijo se reposiciona evaluando la expresión de relación sobre el registro actual del padre—, pero además **acota** el hijo de modo que solo son visibles los registros cuya clave coincide con el valor del padre.

Cuando el padre se mueve, al orden controlador del hijo se le fija el scope superior e inferior igual a la clave de relación, y el hijo se posiciona en el primer registro de ese grupo. La navegación sobre el hijo (`AdsGotoTop`, `AdsGotoBottom`, `AdsSkip`) permanece entonces dentro del grupo coincidente, que es la forma natural de recorrer el lado "muchos" de una relación uno-a-muchos.

El hijo debe tener un orden controlador para que el scope surta efecto; sin él, la relación degrada a un simple movimiento por número de registro (igual que `AdsSetRelation`). `AdsClearRelation` elimina la relación y libera el scope impuesto. Cerrar cualquiera de las tablas también libera el scope. No está disponible para tablas remotas.

## Ejemplo

```c
// Por cada FACTURA, recorrer solo sus propias LÍNEAS.
AdsSetScopedRelation(hFacturas, hLineas, "INV_NO");

AdsGotoTop(hFacturas);
AdsGotoTop(hLineas);
while (1) {
    UNSIGNED16 bEof;
    AdsAtEOF(hLineas, &bEof);    // EOF al final de las líneas de ESTA factura
    if (bEof) break;
    // ... procesar la línea ...
    AdsSkip(hLineas, 1);
}
```

## Ver También

- [AdsSetRelation]({{ site.baseurl }}/es/funciones/ads-set-relation/)
- [AdsClearRelation]({{ site.baseurl }}/es/funciones/ads-clear-relation/)
- [AdsSetScope]({{ site.baseurl }}/es/funciones/ads-set-scope/)

---

[← AdsSetRelation]({{ site.baseurl }}/es/funciones/ads-set-relation/)
