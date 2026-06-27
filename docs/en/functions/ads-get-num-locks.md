---
title: AdsGetNumLocks
layout: default
parent: API Reference
nav_order: 1
permalink: /en/functions/ads-get-num-locks/
---

# AdsGetNumLocks

Returns the count of record locks held on the table.

## Syntax

```c
UNSIGNED32 AdsGetNumLocks(ADSHANDLE hTable, UNSIGNED16 *pusNumLocks);
```

## Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `hTable` | `ADSHANDLE` | Handle of the table. |
| `pusNumLocks` | `UNSIGNED16*` | Output — number of record locks currently held. |

## Return Value

`AE_SUCCESS` (0) on success.

## Description

`AdsGetNumLocks` returns the count of byte-range record locks currently held on the specified table. This includes all individual record locks acquired with `AdsLockRecord`.

For remote tables, the function returns 0 as lock tracking is handled server-side. For local tables, it queries the table's `lock_count()` method to get the current number of held locks.

Note that this does not include table-level exclusive locks — use `AdsIsTableLocked` to check for exclusive table locks.

## Example

```c
ADSHANDLE hTable;
UNSIGNED16 usNumLocks;

AdsOpenTable(&hTable, "customers.dbf", NULL, NULL, ADS_ANSI, ADS_SHARED, 0, 0, NULL);

// Lock some records
AdsLockRecord(hTable, 1);
AdsLockRecord(hTable, 5);
AdsLockRecord(hTable, 10);

AdsGetNumLocks(hTable, &usNumLocks);
printf("Record locks held: %u\n", usNumLocks);  // Output: 3

// Unlock a record
AdsUnlockRecord(hTable, 5);

AdsGetNumLocks(hTable, &usNumLocks);
printf("After unlock: %u\n", usNumLocks);  // Output: 2

AdsCloseTable(hTable);
```

## See Also

- [AdsLockRecord]({{ site.baseurl }}/en/functions/ads-lock-record/)
- [AdsGetAllLocks]({{ site.baseurl }}/en/functions/ads-get-all-locks/)
- [AdsIsTableLocked]({{ site.baseurl }}/en/functions/ads-is-table-locked/)

---

[← AdsGetNumActiveLinks]({{ site.baseurl }}/en/functions/ads-get-num-active-links/)
[AdsGetNumOpenTables →]({{ site.baseurl }}/en/functions/ads-get-num-open-tables/)
