---
title: AdsGetEpoch
layout: default
parent: API Reference
nav_order: 1
permalink: /en/functions/ads-get-epoch/
---

# AdsGetEpoch

Returns the pivot year for 2-digit date interpretation.

## Syntax

```c
UNSIGNED32 AdsGetEpoch(UNSIGNED16 *pusEpoch);
```

## Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pusEpoch` | `UNSIGNED16*` | Output — the pivot year for 2-digit dates. |

## Return Value

`AE_SUCCESS` (0) on success.

## Description

`AdsGetEpoch` returns the current epoch (pivot year) used for interpreting 2-digit years. This setting determines how 2-digit year values are expanded to 4-digit years.

The default epoch is **1900**, meaning:
- 2-digit years 00-99 are interpreted as 1900-1999
- For example, `25` becomes `2025`, `99` becomes `1999`

When the epoch is changed with `AdsSetEpoch`, the interpretation shifts. For example, with epoch 2000:
- 2-digit years 00-49 become 2000-2049
- 2-digit years 50-99 become 1950-1999

This setting affects date parsing and comparison operations throughout the engine.

## Example

```c
UNSIGNED16 usEpoch;

AdsGetEpoch(&usEpoch);
printf("Current epoch (pivot year): %u\n", usEpoch);

// Change epoch to 2000
AdsSetEpoch(2000);
AdsGetEpoch(&usEpoch);
printf("New epoch: %u\n", usEpoch);

// 2-digit year '25' now becomes 2025 instead of 1925
```

## See Also

- [AdsSetEpoch]({{ site.baseurl }}/en/functions/ads-set-epoch/)
- [AdsGetDate]({{ site.baseurl }}/en/functions/ads-get-date/)
- [AdsSetDate]({{ site.baseurl }}/en/functions/ads-set-date/)

---

[← AdsGetDeleted]({{ site.baseurl }}/en/functions/ads-get-deleted/)
[AdsGetExact →]({{ site.baseurl }}/en/functions/ads-get-exact/)
