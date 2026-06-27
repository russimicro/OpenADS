---
title: AdsGetTableConnection
layout: default
parent: API Reference
nav_order: 1
permalink: /en/functions/ads-get-table-connection/
---

# AdsGetTableConnection

Returns the connection handle that owns the table.

## Syntax

```c
UNSIGNED32 AdsGetTableConnection(ADSHANDLE hTable, ADSHANDLE *phConnect);
```

## Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `hTable` | `ADSHANDLE` | Handle of the table. |
| `phConnect` | `ADSHANDLE*` | Output — connection handle that owns the table. |

## Return Value

`AE_SUCCESS` (0) on success.

## Description

`AdsGetTableConnection` returns the connection handle that owns the specified table. This allows you to determine which connection a table belongs to, which is useful when working with multiple connections.

For remote tables, the function searches the handle registry for the `RemoteConnection` handle that matches the table's connection pointer. For local tables, it finds the `Connection` object that owns the table and returns its handle.

If the table is not associated with any connection, the output handle is set to 0.

## Example

```c
ADSHANDLE hConn1, hConn2;
ADSHANDLE hTable1, hTable2;
ADSHANDLE hTableConn;

// Open tables on different connections
AdsConnect60("tcp://server1:6247", NULL, NULL, NULL, 0, &hConn1);
AdsConnect60("tcp://server2:6247", NULL, NULL, NULL, 0, &hConn2);

AdsOpenTable(hConn1, "customers.dbf", NULL, NULL, ADS_ANSI, ADS_SHARED, 0, 0, &hTable1);
AdsOpenTable(hConn2, "orders.dbf", NULL, NULL, ADS_ANSI, ADS_SHARED, 0, 0, &hTable2);

// Get connection for each table
AdsGetTableConnection(hTable1, &hTableConn);
printf("Table1 belongs to connection: %llu\n", hTableConn);

AdsGetTableConnection(hTable2, &hTableConn);
printf("Table2 belongs to connection: %llu\n", hTableConn);

AdsCloseTable(hTable1);
AdsCloseTable(hTable2);
AdsDisconnect(hConn1);
AdsDisconnect(hConn2);
```

## See Also

- [AdsConnect60]({{ site.baseurl }}/en/functions/ads-connect60/)
- [AdsOpenTable]({{ site.baseurl }}/en/functions/ads-open-table/)
- [AdsGetConnectionType]({{ site.baseurl }}/en/functions/ads-get-connection-type/)

---

[← AdsGetTableCharType]({{ site.baseurl }}/en/functions/ads-get-table-char-type/)
[AdsGetTableOpenOptions →]({{ site.baseurl }}/en/functions/ads-get-table-open-options/)
