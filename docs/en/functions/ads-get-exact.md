---
title: AdsGetExact
layout: default
parent: API Reference
nav_order: 1
permalink: /en/functions/ads-get-exact/
---

# AdsGetExact

Returns the SET EXACT state (0=OFF, 1=ON).

## Syntax

```c
UNSIGNED32 AdsGetExact(UNSIGNED16 *pusExact);
```

## Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pusExact` | `UNSIGNED16*` | Output — 0 if SET EXACT is OFF, 1 if ON. |

## Return Value

`AE_SUCCESS` (0) on success.

## Description

`AdsGetExact` returns whether `SET EXACT` is currently enabled or disabled.

- **0 (OFF)**: String comparisons use the traditional behavior where `"ABC" = "ABCDEF"` is true (trailing characters are ignored)
- **1 (ON)**: String comparisons require exact matches where `"ABC" = "ABCDEF"` is false

This setting affects how string comparisons work in seek operations, filter expressions, and other comparison contexts. When SET EXACT is OFF, a comparison `cExpression1 == cExpression2` is true if `cExpression2` is a prefix of `cExpression1`.

## Example

```c
UNSIGNED16 usExact;

AdsGetExact(&usExact);
printf("SET EXACT is %s\n", usExact ? "ON" : "OFF");

// Enable exact matching
AdsSetExact(1);
AdsGetExact(&usExact);
printf("After AdsSetExact(1): %s\n", usExact ? "ON" : "OFF");

// Now seek operations require exact key matches
AdsSeek(hTable, "ABC", ADS_SEEK_EXACT);
```

## See Also

- [AdsSetExact]({{ site.baseurl }}/en/functions/ads-set-exact/)
- [AdsSeek]({{ site.baseurl }}/en/functions/ads-seek/)
- [AdsSeekLast]({{ site.baseurl }}/en/functions/ads-seek-last/)

---

[← AdsGetEpoch]({{ site.baseurl }}/en/functions/ads-get-epoch/)
[AdsGetFilter →]({{ site.baseurl }}/en/functions/ads-get-filter/)
