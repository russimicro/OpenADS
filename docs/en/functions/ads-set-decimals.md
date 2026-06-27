---
title: AdsSetDecimals
layout: default
parent: API Reference
nav_order: 1
permalink: /en/functions/ads-set-decimals/
---

# AdsSetDecimals

Sets the default number of decimals recorded for the session.

## Syntax

```c
UNSIGNED32 AdsSetDecimals(UNSIGNED16 usDecimals);
```

## Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `usDecimals` | `UNSIGNED16` | Default number of decimal places. |

## Return Value

`AE_SUCCESS` (0).

## Description

`AdsSetDecimals` records the default decimal-place count (the SET DECIMALS setting). It is stored for ACE API parity; OpenADS reads and writes numeric fields at their schema-defined precision regardless of this value.

## Example

```c
AdsSetDecimals(4);
```

## See Also

- [AdsSetExact]({{ site.baseurl }}/en/functions/ads-set-exact/)
- [AdsSetEpoch]({{ site.baseurl }}/en/functions/ads-set-epoch/)

---

[AdsSetExact →]({{ site.baseurl }}/en/functions/ads-set-exact/)
