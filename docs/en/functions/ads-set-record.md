---
title: AdsSetRecord
layout: default
parent: API Reference
nav_order: 1
permalink: /en/functions/ads-set-record/
---

# AdsSetRecord

Overwrites the current record with a raw physical record image.

## Syntax

```c
UNSIGNED32 AdsSetRecord(ADSHANDLE hTable, UNSIGNED8 *pucRecord, UNSIGNED32 ulLen);
```

## Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `hTable` | `ADSHANDLE` | Handle of the table. |
| `pucRecord` | `UNSIGNED8*` | Buffer holding the raw record image (deletion-flag byte + field bytes). |
| `ulLen` | `UNSIGNED32` | Length of the supplied image, in bytes. |

## Return Value

`AE_SUCCESS` (0) on success. `AE_NO_CURRENT_RECORD` (5068) when the cursor is at BOF/EOF. `AE_INTERNAL_ERROR` (5000) for a null buffer, a read-only table, or an unknown handle.

## Description

`AdsSetRecord` writes a complete physical record image — as produced by `AdsGetRecord` — over the current record, then flushes it to disk and re-synchronises every bound index so any change to a key field is reflected in the index order.

At most *record length* bytes are copied; if `ulLen` is shorter, only that many bytes are written and the tail of the record is left intact. The leading byte of the image is the deletion flag (a space for an active record, `*` for a deleted one).

This is the write counterpart of `AdsGetRecord`. It is not available for remote tables.

## Example

```c
ADSHANDLE hTable;
UNSIGNED32 ulLen = 0;

AdsGetRecord(hTable, NULL, &ulLen);
UNSIGNED8 *pucRec = malloc(ulLen);
AdsGetRecord(hTable, pucRec, &ulLen);

// Patch a fixed-width field in place and write it back.
memcpy(pucRec + 5, "BBBBBBBB", 8);
AdsSetRecord(hTable, pucRec, ulLen);

free(pucRec);
```

## See Also

- [AdsGetRecord]({{ site.baseurl }}/en/functions/ads-get-record/)
- [AdsWriteRecord]({{ site.baseurl }}/en/functions/ads-write-record/)
- [AdsAppendRecord]({{ site.baseurl }}/en/functions/ads-append-record/)
- [AdsGetRecordLength]({{ site.baseurl }}/en/functions/ads-get-record-length/)

---

[← AdsGetRecord]({{ site.baseurl }}/en/functions/ads-get-record/)
