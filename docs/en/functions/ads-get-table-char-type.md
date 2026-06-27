---
title: AdsGetTableCharType
layout: default
parent: API Reference
nav_order: 1
permalink: /en/functions/ads-get-table-char-type/
---

# AdsGetTableCharType

Returns the character type of the table (ANSI or OEM).

## Syntax

```c
UNSIGNED32 AdsGetTableCharType(ADSHANDLE hTable, UNSIGNED16 *pusCharType);
```

## Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `hTable` | `ADSHANDLE` | Handle of the table. |
| `pusCharType` | `UNSIGNED16*` | Output — character type constant (`ADS_ANSI` or `ADS_OEM`). |

## Return Value

`AE_SUCCESS` (0) on success.

## Description

`AdsGetTableCharType` returns the character type of the table. In OpenADS, this always returns `ADS_ANSI` (1) because OpenADS exclusively uses ANSI character encoding.

The character type determines how character data is stored and compared in the table. Traditional dBASE/FoxPro tables used OEM character encoding, while modern applications typically use ANSI.

- `ADS_ANSI` (1): ANSI character encoding (Windows code page)
- `ADS_OEM` (2): OEM character encoding (DOS code page)

Note that OpenADS does not support OEM character type — all tables are treated as ANSI.

## Example

```c
ADSHANDLE hTable;
UNSIGNED16 usCharType;

AdsOpenTable(&hTable, "customers.dbf", NULL, NULL, ADS_ANSI, ADS_SHARED, 0, 0, NULL);

AdsGetTableCharType(hTable, &usCharType);
if (usCharType == ADS_ANSI)
    printf("Table uses ANSI character encoding\n");
else if (usCharType == ADS_OEM)
    printf("Table uses OEM character encoding\n");

AdsCloseTable(hTable);
```

## See Also

- [AdsOpenTable]({{ site.baseurl }}/en/functions/ads-open-table/)
- [AdsGetTableType]({{ site.baseurl }}/en/functions/ads-get-table-type/)

---

[← AdsGetTableAlias]({{ site.baseurl }}/en/functions/ads-get-table-alias/)
[AdsGetTableConType →]({{ site.baseurl }}/en/functions/ads-get-table-con-type/)
