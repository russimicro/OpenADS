---
title: AdsSetScopedRelation
layout: default
parent: API Reference
nav_order: 1
permalink: /en/functions/ads-set-scoped-relation/
---

# AdsSetScopedRelation

Establishes a parent-to-child relation that also scopes the child to the matching key group.

## Syntax

```c
UNSIGNED32 AdsSetScopedRelation(ADSHANDLE hTableParent, ADSHANDLE hTableChild, UNSIGNED8 *pucExpr);
```

## Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `hTableParent` | `ADSHANDLE` | Handle of the parent (controlling) table. |
| `hTableChild` | `ADSHANDLE` | Handle of the child (related) table. |
| `pucExpr` | `UNSIGNED8*` | Relation expression, evaluated against the parent's current record to produce the child key. |

## Return Value

`AE_SUCCESS` (0) on success. `AE_FUNCTION_NOT_AVAILABLE` (5004) for remote tables. `AE_INTERNAL_ERROR` (5000) for a null expression or an unknown handle.

## Description

`AdsSetScopedRelation` works exactly like `AdsSetRelation` — moving the parent re-positions the child by evaluating the relation expression against the parent's current record — but it additionally **scopes** the child so that only the records whose key matches the parent value are visible.

When the parent moves, the child's controlling order has its top and bottom scope set to the relation key, and the child is positioned on the first record in that group. Navigation on the child (`AdsGotoTop`, `AdsGotoBottom`, `AdsSkip`) then stays within the matching group, which is the natural way to walk the "many" side of a one-to-many relationship.

The child must have a controlling order for the scope to take effect; without one, the relation degrades to a plain record-number move (same as `AdsSetRelation`). `AdsClearRelation` removes the relation and releases the scope it imposed. Closing either table also releases the scope. This function is not available for remote tables.

## Example

```c
// For each INVOICE, walk only its own LINE items.
AdsSetScopedRelation(hInvoices, hLines, "INV_NO");

AdsGotoTop(hInvoices);
AdsGotoTop(hLines);
while (1) {
    UNSIGNED16 bEof;
    AdsAtEOF(hLines, &bEof);     // EOF at the end of THIS invoice's lines
    if (bEof) break;
    // ... process the line item ...
    AdsSkip(hLines, 1);
}
```

## See Also

- [AdsSetRelation]({{ site.baseurl }}/en/functions/ads-set-relation/)
- [AdsClearRelation]({{ site.baseurl }}/en/functions/ads-clear-relation/)
- [AdsSetScope]({{ site.baseurl }}/en/functions/ads-set-scope/)

---

[← AdsSetRelation]({{ site.baseurl }}/en/functions/ads-set-relation/)
