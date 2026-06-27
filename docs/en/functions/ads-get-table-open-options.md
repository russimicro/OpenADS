---
title: AdsGetTableOpenOptions
layout: default
parent: API Reference
nav_order: 1
permalink: /en/functions/ads-get-table-open-options/
---

# AdsGetTableOpenOptions

Returns the open mode flags of the table.

## Syntax

```c
UNSIGNED32 AdsGetTableOpenOptions(ADSHANDLE hTable, UNSIGNED32 *pulOptions);
```

## Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `hTable` | `ADSHANDLE` | Handle of the table. |
| `pulOptions` | `UNSIGNED32*` | Output — open mode flags. |

## Return Value

`AE_SUCCESS` (0) on success.

## Description

`AdsGetTableOpenOptions` returns the open mode flags that were used when the table was opened. These flags indicate the access mode and sharing behavior of the table.

| Constant | Value | Description |
|----------|-------|-------------|
| `ADS_EXCLUSIVE` | 1 | Table opened exclusively (no other users can access) |
| `ADS_SHARED` | 2 | Table opened shared (other users can read/write) |
| `ADS_READONLY` | 3 | Table opened read-only |

The function maps the internal `OpenMode` enum to the corresponding ACE open-option constants. For remote tables, the function returns 0 as the open mode is managed server-side.

## Example

```c
ADSHANDLE hTable;
UNSIGNED32 ulOptions;

// Open table in shared mode
AdsOpenTable(&hTable, "customers.dbf", NULL, NULL, ADS_ANSI, ADS_SHARED, 0, 0, NULL);

AdsGetTableOpenOptions(hTable, &ulOptions);
switch (ulOptions) {
    case ADS_EXCLUSIVE:
        printf("Table opened exclusively\n");
        break;
    case ADS_SHARED:
        printf("Table opened shared\n");
        break;
    case ADS_READONLY:
        printf("Table opened read-only\n");
        break;
}

AdsCloseTable(hTable);
```

## See Also

- [AdsOpenTable]({{ site.baseurl }}/en/functions/ads-open-table/)
- [AdsGetTableType]({{ site.baseurl }}/en/functions/ads-get-table-type/)

---

[← AdsGetTableConnection]({{ site.baseurl }}/en/functions/ads-get-table-connection/)
[AdsIsConnectionAlive →]({{ site.baseurl }}/en/functions/ads-is-connection-alive/)
