---
title: AdsGetFilter
layout: default
parent: API Reference
nav_order: 1
permalink: /en/functions/ads-get-filter/
---

# AdsGetFilter

Returns the current filter expression string.

## Syntax

```c
UNSIGNED32 AdsGetFilter(ADSHANDLE hTable, UNSIGNED8 *pucFilter, UNSIGNED16 *pusFilterLen);
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

`AdsGetFilter` returns the current filter expression string that was set with `AdsSetFilter`. The filter expression determines which records are visible during operations like browsing, seeking, and navigating.

If no filter is active on the table, an empty string is returned. The filter expression is stored as a string and can be retrieved for inspection or modification.

Note that in OpenADS, `AdsSetFilter` is a no-op stub that returns success but does not actually apply the filter. However, `AdsGetFilter` will return any filter expression that was stored.

For Rushmore-optimized filters, use `AdsGetAOF` instead.

## Example

```c
ADSHANDLE hTable;
UNSIGNED8 aucFilter[256];
UNSIGNED16 usLen = sizeof(aucFilter);

AdsOpenTable(&hTable, "customers.dbf", NULL, NULL, ADS_ANSI, ADS_SHARED, 0, 0, NULL);

// Try to set a filter (note: AdsSetFilter is a stub in OpenADS)
AdsSetFilter(hTable, "COUNTRY = 'USA'");

// Retrieve the filter expression
AdsGetFilter(hTable, aucFilter, &usLen);
printf("Active filter: %s\n", aucFilter);

AdsCloseTable(hTable);
```

## See Also

- [AdsSetFilter]({{ site.baseurl }}/en/functions/ads-set-filter/)
- [AdsClearFilter]({{ site.baseurl }}/en/functions/ads-clear-filter/)
- [AdsGetAOF]({{ site.baseurl }}/en/functions/ads-get-aof/)

---

[← AdsGetExact]({{ site.baseurl }}/en/functions/ads-get-exact/)
[AdsGetHandleType →]({{ site.baseurl }}/en/functions/ads-get-handle-type/)
