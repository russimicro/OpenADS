---
title: AdsGetTableAlias
layout: default
parent: API Reference
nav_order: 1
permalink: /en/functions/ads-get-table-alias/
---

# AdsGetTableAlias

Returns the alias (stem name without extension) of the open table.

## Syntax

```c
UNSIGNED32 AdsGetTableAlias(ADSHANDLE hTable, UNSIGNED8 *pucAlias, UNSIGNED16 *pusAliasLen);
```

## Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `hTable` | `ADSHANDLE` | Handle of the table. |
| `pucAlias` | `UNSIGNED8*` | Caller-allocated buffer to receive the alias string. |
| `pusAliasLen` | `UNSIGNED16*` | Input/output — buffer capacity on input, actual length on output. |

## Return Value

`AE_SUCCESS` (0) on success.

## Description

`AdsGetTableAlias` returns the alias of the open table. The alias is the stem name of the table file without the extension. For example, if the table file is `CUSTOMERS.DBF`, the alias would be `CUSTOMERS`.

For remote tables, the alias is stored in the remote table structure. For local tables, it is retrieved from the table's `alias()` method.

If an explicit alias was provided when opening the table with `AdsOpenTable`, that alias is returned. Otherwise, the default alias (filename stem) is used.

## Example

```c
ADSHANDLE hTable;
UNSIGNED8 aucAlias[256];
UNSIGNED16 usLen = sizeof(aucAlias);

// Open with explicit alias
AdsOpenTable(&hTable, "customers.dbf", "CUST", NULL, ADS_ANSI, ADS_SHARED, 0, 0, NULL);

AdsGetTableAlias(hTable, aucAlias, &usLen);
printf("Table alias: %s\n", aucAlias);  // Output: CUST

AdsCloseTable(hTable);

// Open without explicit alias
AdsOpenTable(&hTable, "orders.dbf", NULL, NULL, ADS_ANSI, ADS_SHARED, 0, 0, NULL);

AdsGetTableAlias(hTable, aucAlias, &usLen);
printf("Table alias: %s\n", aucAlias);  // Output: ORDERS

AdsCloseTable(hTable);
```

## See Also

- [AdsOpenTable]({{ site.baseurl }}/en/functions/ads-open-table/)
- [AdsGetTableFilename]({{ site.baseurl }}/en/functions/ads-get-table-filename/)
- [AdsGetTableType]({{ site.baseurl }}/en/functions/ads-get-table-type/)

---

[← AdsGetNumOpenTables]({{ site.baseurl }}/en/functions/ads-get-num-open-tables/)
[AdsGetTableCharType →]({{ site.baseurl }}/en/functions/ads-get-table-char-type/)
