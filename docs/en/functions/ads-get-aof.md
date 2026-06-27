---
title: AdsGetAOF
layout: default
parent: API Reference
nav_order: 1
permalink: /en/functions/ads-get-aof/
---

# AdsGetAOF

Returns the current Advanced Optimized Filter (AOF/Rushmore) expression string.

## Syntax

```c
UNSIGNED32 AdsGetAOF(ADSHANDLE hTable, UNSIGNED8 *pucFilter, UNSIGNED16 *pusFilterLen);
```

## Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `hTable` | `ADSHANDLE` | Handle of the table. |
| `pucFilter` | `UNSIGNED8*` | Caller-allocated buffer to receive the filter expression string. |
| `pusFilterLen` | `UNSIGNED16*` | Input/output — buffer capacity on input, actual length on output. |

## Return Value

`AE_SUCCESS` (0) on success.

## Description

`AdsGetAOF` returns the current AOF (Advanced Optimized Filter) expression string that was set with `AdsSetAOF`. AOF is a Rushmore-style optimized filter that uses index data for fast record selection.

If no AOF is active on the table, an empty string is returned. For remote tables, the filter expression is stored on the client side and returned directly.

The AOF expression is different from a regular filter set with `AdsSetFilter` — AOF expressions are optimized by the engine to use available indexes for faster record retrieval.

## Example

```c
ADSHANDLE hTable;
UNSIGNED8 aucFilter[256];
UNSIGNED16 usLen = sizeof(aucFilter);

// Open table and set AOF
AdsOpenTable(&hTable, "customers.dbf", NULL, NULL, ADS_ANSI, ADS_SHARED, 0, 0, NULL);
AdsSetAOF(hTable, "COUNTRY = 'USA'", ADS_OPTIMIZE_FULL);

// Retrieve the AOF expression
AdsGetAOF(hTable, aucFilter, &usLen);
printf("Active AOF: %s\n", aucFilter);

AdsCloseTable(hTable);
```

## See Also

- [AdsSetAOF]({{ site.baseurl }}/en/functions/ads-set-aof/)
- [AdsClearAOF]({{ site.baseurl }}/en/functions/ads-clear-aof/)
- [AdsGetAOFOptLevel]({{ site.baseurl }}/en/functions/ads-get-aof-opt-level/)
- [AdsGetFilter]({{ site.baseurl }}/en/functions/ads-get-filter/)

---

[AdsGetConnectionType →]({{ site.baseurl }}/en/functions/ads-get-connection-type/)
