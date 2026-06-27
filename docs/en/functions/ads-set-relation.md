---
title: AdsSetRelation
layout: default
parent: API Reference
nav_order: 1
permalink: /en/functions/ads-set-relation/
---

# AdsSetRelation

Establishes a parent-to-child work-area relation so the child follows the parent.

## Syntax

```c
UNSIGNED32 AdsSetRelation(ADSHANDLE hTableParent, ADSHANDLE hTableChild, UNSIGNED8 *pucExpr);
```

## Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `hTableParent` | `ADSHANDLE` | Handle of the parent (controlling) table. |
| `hTableChild` | `ADSHANDLE` | Handle of the child (related) table. |
| `pucExpr` | `UNSIGNED8*` | Relation expression, evaluated against the parent's current record to produce the child seek key. |

## Return Value

`AE_SUCCESS` (0) on success. `AE_FUNCTION_NOT_AVAILABLE` (5004) for remote tables. `AE_INTERNAL_ERROR` (5000) for a null expression or an unknown handle.

## Description

`AdsSetRelation` links a child table to a parent so that whenever the parent's cursor moves, the child is automatically re-positioned. After each parent navigation (`AdsGotoTop`, `AdsGotoBottom`, `AdsSkip`, `AdsGotoRecord`, `AdsSeek`), the relation expression is evaluated against the parent's current record and the result is used to seek the child:

- If the child has a controlling order, the value is sought in that index (soft seek). A miss leaves the child at EOF.
- If the child has no controlling order, the value is treated as a record number and the child is moved to it.

The child is positioned immediately when the relation is set, using the parent's current record. Multiple children may be related to the same parent, and relations cascade through chained parent-child links. A parent may set more than one relation; call `AdsClearRelation` to remove them. Closing either table drops the affected relations. This function is not available for remote tables.

## Example

```c
// Parent ORDERS, child CUSTOMERS indexed on its numeric ID.
AdsSetRelation(hOrders, hCustomers, "CUST_ID");

AdsGotoTop(hOrders);          // child auto-seeks to the matching customer
// ... read fields from hCustomers for the current order ...
AdsSkip(hOrders, 1);          // child follows automatically
```

## See Also

- [AdsClearRelation]({{ site.baseurl }}/en/functions/ads-clear-relation/)
- [AdsSeek]({{ site.baseurl }}/en/functions/ads-seek/)
- [AdsSetIndexOrder]({{ site.baseurl }}/en/functions/ads-set-index-order/)

---

[AdsClearRelation →]({{ site.baseurl }}/en/functions/ads-clear-relation/)
