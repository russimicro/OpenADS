---
title: AdsGetRecordCRC
layout: default
parent: API Reference
nav_order: 1
permalink: /en/functions/ads-get-record-crc/
---

# AdsGetRecordCRC

Computes a CRC checksum of the current record image.

## Syntax

```c
UNSIGNED32 AdsGetRecordCRC(ADSHANDLE hTable, UNSIGNED32 *pulCRC, UNSIGNED32 ulOptions);
```

## Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `hTable` | `ADSHANDLE` | Handle of the table. |
| `pulCRC` | `UNSIGNED32*` | Receives the 32-bit CRC of the current record. |
| `ulOptions` | `UNSIGNED32` | Reserved; pass 0. |

## Return Value

`AE_SUCCESS` (0) on success. `AE_NO_CURRENT_RECORD` (5068) when the cursor is at BOF/EOF. `AE_FUNCTION_NOT_AVAILABLE` (5004) for remote tables.

## Description

`AdsGetRecordCRC` returns a 32-bit checksum computed over the raw physical record image — the same bytes `AdsGetRecord` returns, including the leading deletion-flag byte. It uses the standard IEEE CRC-32 (reflected, polynomial `0xEDB88320`).

The checksum is a fast way to detect whether a record has changed: read the CRC, do other work, read it again, and compare. Two records with identical field bytes produce the same CRC; any difference in the image yields a different value. The value is stable for a given record image but is **not** guaranteed to match the checksum any other ADS implementation computes.

This function is not available for remote tables.

## Example

```c
UNSIGNED32 ulBefore = 0, ulAfter = 0;

AdsGetRecordCRC(hTable, &ulBefore, 0);
// ... another process may update the row ...
AdsRefreshRecord(hTable);
AdsGetRecordCRC(hTable, &ulAfter, 0);

if (ulBefore != ulAfter)
    printf("record changed\n");
```

## See Also

- [AdsGetRecord]({{ site.baseurl }}/en/functions/ads-get-record/)
- [AdsRefreshRecord]({{ site.baseurl }}/en/functions/ads-refresh-record/)
- [AdsGetRecordLength]({{ site.baseurl }}/en/functions/ads-get-record-length/)

---

[AdsGetRecord →]({{ site.baseurl }}/en/functions/ads-get-record/)
