---
title: AdsClearRelation
layout: default
parent: API Reference
nav_order: 1
permalink: /en/functions/ads-clear-relation/
---

# AdsClearRelation

Removes all relations set from a parent table.

## Syntax

```c
UNSIGNED32 AdsClearRelation(ADSHANDLE hTableParent);
```

## Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `hTableParent` | `ADSHANDLE` | Handle of the parent table whose relations are to be cleared. |

## Return Value

`AE_SUCCESS` (0) on success.

## Description

`AdsClearRelation` removes every parent-to-child relation previously established from `hTableParent` with `AdsSetRelation`. After the call, moving the parent's cursor no longer re-positions the formerly related children; each child keeps its current position.

Only the relations this table drives **as a parent** are removed. If the same table is also a child of another work area, that binding is left intact. Relations are also dropped automatically when either the parent or a child table is closed.

## Example

```c
AdsSetRelation(hOrders, hCustomers, "CUST_ID");
// ... browse with the relation active ...

AdsClearRelation(hOrders);    // child no longer follows the parent
```

## See Also

- [AdsSetRelation]({{ site.baseurl }}/en/functions/ads-set-relation/)
- [AdsClearScope]({{ site.baseurl }}/en/functions/ads-clear-scope/)

---

[← AdsSetRelation]({{ site.baseurl }}/en/functions/ads-set-relation/)
