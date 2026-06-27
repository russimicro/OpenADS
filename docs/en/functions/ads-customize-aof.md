---
title: AdsCustomizeAOF
layout: default
parent: API Reference
nav_order: 1
permalink: /en/functions/ads-customize-aof/
---

# AdsCustomizeAOF

Forces individual records into or out of the active Advantage Optimized Filter.

## Syntax

```c
UNSIGNED32 AdsCustomizeAOF(ADSHANDLE hTable, UNSIGNED32 ulNumRecords,
                           UNSIGNED32 *pulRecords, UNSIGNED16 usOption);
```

## Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `hTable` | `ADSHANDLE` | Handle of the table with an active AOF. |
| `ulNumRecords` | `UNSIGNED32` | Number of record numbers in `pulRecords`. |
| `pulRecords` | `UNSIGNED32*` | Array of record numbers to add or remove. |
| `usOption` | `UNSIGNED16` | `ADS_AOF_ADD_RECORD` (1) to include the records, `ADS_AOF_REMOVE_RECORD` (2) to exclude them. |

## Return Value

`AE_SUCCESS` (0) on success. `AE_INTERNAL_ERROR` (5000) if no AOF is active, the option is invalid, or the handle is unknown. `AE_FUNCTION_NOT_AVAILABLE` (5004) for remote tables.

## Description

`AdsCustomizeAOF` manually overrides the membership of specific records in the AOF (Advanced Optimized Filter) result set installed by `AdsSetAOF`. `ADS_AOF_ADD_RECORD` makes the listed records visible even if they did not satisfy the filter expression; `ADS_AOF_REMOVE_RECORD` hides them even if they did.

The change updates the retained AOF bitmap and reinstalls it, so subsequent navigation (`AdsGotoTop`, `AdsGotoBottom`, `AdsSkip`) immediately reflects the customized set. Record numbers outside the table range are ignored. The customization is discarded when the AOF is cleared with `AdsClearAOF` or replaced by a new `AdsSetAOF`.

This function requires an active AOF and is not available for remote tables.

## Example

```c
AdsSetAOF(hTable, "BALANCE > 0", 0);

// Always include record 4, even though its balance is 0.
UNSIGNED32 add[] = { 4 };
AdsCustomizeAOF(hTable, 1, add, ADS_AOF_ADD_RECORD);

// Hide record 2 regardless of its balance.
UNSIGNED32 rem[] = { 2 };
AdsCustomizeAOF(hTable, 1, rem, ADS_AOF_REMOVE_RECORD);
```

## See Also

- [AdsSetAOF]({{ site.baseurl }}/en/functions/ads-set-aof/)
- [AdsGetAOF]({{ site.baseurl }}/en/functions/ads-get-aof/)
- [AdsClearAOF]({{ site.baseurl }}/en/functions/ads-clear-aof/)

---

[AdsGetAOF →]({{ site.baseurl }}/en/functions/ads-get-aof/)
