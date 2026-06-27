---
title: AdsGetRecord
layout: default
parent: API Reference
nav_order: 1
permalink: /en/functions/ads-get-record/
---

# AdsGetRecord

Copies the raw physical record image of the current row into a caller buffer.

## Syntax

```c
UNSIGNED32 AdsGetRecord(ADSHANDLE hTable, UNSIGNED8 *pucRecord, UNSIGNED32 *pulLen);
```

## Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `hTable` | `ADSHANDLE` | Handle of the table. |
| `pucRecord` | `UNSIGNED8*` | Caller-allocated buffer to receive the raw record image. May be `NULL` to query the required size. |
| `pulLen` | `UNSIGNED32*` | Input/output — buffer capacity on input, number of bytes copied on output. |

## Return Value

`AE_SUCCESS` (0) on success. `AE_INSUFFICIENT_BUFFER` (5051) if the buffer is smaller than the record; the required length is written back through `pulLen`. `AE_NO_CURRENT_RECORD` (5068) when the cursor is at BOF/EOF.

## Description

`AdsGetRecord` returns the complete physical record image exactly as it is stored in the table: the leading deletion-flag byte (a space for an active record, `*` for a deleted one) followed by the raw field bytes. The data is copied verbatim and is **not** NUL-terminated, so it may contain embedded zero or high bytes.

If `pucRecord` is `NULL` or `*pulLen` is 0, the call is treated as a size query: the record length is written to `*pulLen` and no bytes are copied, so the caller can allocate the exact buffer and call again.

This function is the counterpart of `AdsSetRecord`, which writes a raw image back over the current record. It is not available for remote tables.

## Example

```c
ADSHANDLE hTable;
UNSIGNED32 ulLen = 0;

// Size query, then read.
AdsGetRecord(hTable, NULL, &ulLen);
UNSIGNED8 *pucRec = malloc(ulLen);
AdsGetRecord(hTable, pucRec, &ulLen);

printf("deleted=%d, %u bytes\n", pucRec[0] == '*', ulLen);
free(pucRec);
```

## See Also

- [AdsSetRecord]({{ site.baseurl }}/en/functions/ads-set-record/)
- [AdsGetField]({{ site.baseurl }}/en/functions/ads-get-field/)
- [AdsGetRecordLength]({{ site.baseurl }}/en/functions/ads-get-record-length/)
- [AdsIsRecordDeleted]({{ site.baseurl }}/en/functions/ads-is-record-deleted/)

---

[AdsSetRecord →]({{ site.baseurl }}/en/functions/ads-set-record/)
