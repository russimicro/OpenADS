---
title: AdsIsTableLocked
layout: default
parent: API Reference
nav_order: 1
permalink: /en/functions/ads-is-table-locked/
---

# AdsIsTableLocked

Returns whether the table has an exclusive lock.

## Syntax

```c
UNSIGNED32 AdsIsTableLocked(ADSHANDLE hTable, UNSIGNED16 *pbLocked);
```

## Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `hTable` | `ADSHANDLE` | Handle of the table. |
| `pbLocked` | `UNSIGNED16*` | Output — 1 if table is exclusively locked, 0 otherwise. |

## Return Value

`AE_SUCCESS` (0) on success.

## Description

`AdsIsTableLocked` checks whether the specified table currently has an exclusive table lock. An exclusive lock prevents other users from accessing the table.

For remote tables, the function returns 0 as table lock status is managed server-side. For local tables, it queries the table's `is_table_locked()` method.

Note that this checks for exclusive TABLE locks only, not individual record locks. To check record locks, use `AdsIsRecordLocked` or `AdsGetNumLocks`.

## Example

```c
ADSHANDLE hTable;
UNSIGNED16 bLocked;

AdsOpenTable(&hTable, "customers.dbf", NULL, NULL, ADS_ANSI, ADS_SHARED, 0, 0, NULL);

AdsIsTableLocked(hTable, &bLocked);
printf("Table locked: %s\n", bLocked ? "yes" : "no");

// Acquire exclusive lock
AdsLockTable(hTable);

AdsIsTableLocked(hTable, &bLocked);
printf("After LockTable: %s\n", bLocked ? "yes" : "no");

// Release lock
AdsUnlockTable(hTable);

AdsIsTableLocked(hTable, &bLocked);
printf("After UnlockTable: %s\n", bLocked ? "yes" : "no");

AdsCloseTable(hTable);
```

## See Also

- [AdsLockTable]({{ site.baseurl }}/en/functions/ads-lock-table/)
- [AdsUnlockTable]({{ site.baseurl }}/en/functions/ads-unlock-table/)
- [AdsGetNumLocks]({{ site.baseurl }}/en/functions/ads-get-num-locks/)

---

[← AdsIsNull]({{ site.baseurl }}/en/functions/ads-is-null/)
