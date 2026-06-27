---
title: AdsTestRecLocks
layout: default
parent: API Reference
nav_order: 1
permalink: /en/functions/ads-test-rec-locks/
---

# AdsTestRecLocks

Diagnostic hook for a table's record locks.

## Syntax

```c
UNSIGNED32 AdsTestRecLocks(ADSHANDLE hTable);
```

## Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `hTable` | `ADSHANDLE` | Handle of the table. |

## Return Value

`AE_SUCCESS` (0) on success. `AE_INTERNAL_ERROR` (5000) for an unknown handle.

## Description

`AdsTestRecLocks` is a diagnostic hook. OpenADS has no separate lock-table consistency check to run, so the call validates the table handle and reports success. It is provided for ACE API compatibility. To inspect actual lock state, use `AdsGetAllLocks` or `AdsIsRecordLocked`.

## Example

```c
AdsTestRecLocks(hTable);
```

## See Also

- [AdsGetAllLocks]({{ site.baseurl }}/en/functions/ads-get-all-locks/)
- [AdsIsRecordLocked]({{ site.baseurl }}/en/functions/ads-is-record-locked/)
- [AdsGetNumLocks]({{ site.baseurl }}/en/functions/ads-get-num-locks/)

---

[AdsGetNumLocks →]({{ site.baseurl }}/en/functions/ads-get-num-locks/)
