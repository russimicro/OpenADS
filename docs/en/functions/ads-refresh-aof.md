---
title: AdsRefreshAOF
layout: default
parent: API Reference
nav_order: 1
permalink: /en/functions/ads-refresh-aof/
---

# AdsRefreshAOF

Re-evaluates the active Advantage Optimized Filter against current data.

## Syntax

```c
UNSIGNED32 AdsRefreshAOF(ADSHANDLE hTable);
```

## Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `hTable` | `ADSHANDLE` | Handle of the table with an active AOF. |

## Return Value

`AE_SUCCESS` (0) on success, including when no AOF is active (no-op). `AE_INTERNAL_ERROR` (5000) for an unknown handle.

## Description

`AdsRefreshAOF` re-evaluates the AOF expression installed by `AdsSetAOF` against the table's current contents and reinstalls the resulting bitmap. Records that were appended or whose key fields changed since the filter was set are re-classified, so the visible set reflects the latest data.

If no AOF is active, or the set was built only via `AdsCustomizeAOF` (no stored expression), the call succeeds without changing anything. For remote tables the server maintains the AOF, so the call is a no-op.

## Example

```c
AdsSetAOF(hTable, "BALANCE > 0", 0);
// ... records change or are appended ...
AdsRefreshAOF(hTable);   // visible set now reflects the new data
```

## See Also

- [AdsSetAOF]({{ site.baseurl }}/en/functions/ads-set-aof/)
- [AdsCustomizeAOF]({{ site.baseurl }}/en/functions/ads-customize-aof/)
- [AdsClearAOF]({{ site.baseurl }}/en/functions/ads-clear-aof/)

---

[AdsCustomizeAOF →]({{ site.baseurl }}/en/functions/ads-customize-aof/)
