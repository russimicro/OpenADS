---
title: AdsCacheRecords
layout: default
parent: API Reference
nav_order: 1
permalink: /en/functions/ads-cache-records/
---

# AdsCacheRecords

Hints how many records to read ahead for a table.

## Syntax

```c
UNSIGNED32 AdsCacheRecords(ADSHANDLE hTable, UNSIGNED16 usNumRecords);
```

## Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `hTable` | `ADSHANDLE` | Handle of the table. |
| `usNumRecords` | `UNSIGNED16` | Suggested number of records to read ahead. |

## Return Value

`AE_SUCCESS` (0) on success. `AE_INTERNAL_ERROR` (5000) for an unknown handle.

## Description

`AdsCacheRecords` is an advisory read-ahead hint. OpenADS does not pre-cache rows, so the call validates the table handle and succeeds without changing behaviour. It is provided for ACE API compatibility with code that tunes client-side caching.

## Example

```c
AdsCacheRecords(hTable, 50);
```

## See Also

- [AdsCacheOpenTables]({{ site.baseurl }}/en/functions/ads-cache-open-tables/)
- [AdsCacheOpenCursors]({{ site.baseurl }}/en/functions/ads-cache-open-cursors/)

---

[AdsCacheOpenTables →]({{ site.baseurl }}/en/functions/ads-cache-open-tables/)
