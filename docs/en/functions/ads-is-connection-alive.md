---
title: AdsIsConnectionAlive
layout: default
parent: API Reference
nav_order: 1
permalink: /en/functions/ads-is-connection-alive/
---

# AdsIsConnectionAlive

Returns whether the connection is alive (always 1 for local).

## Syntax

```c
UNSIGNED32 AdsIsConnectionAlive(ADSHANDLE hConnect, UNSIGNED16 *pbAlive);
```

## Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `hConnect` | `ADSHANDLE` | Handle of the connection or any object derived from it. |
| `pbAlive` | `UNSIGNED16*` | Output — 1 if connection is alive, 0 otherwise. |

## Return Value

`AE_SUCCESS` (0) on success.

## Description

`AdsIsConnectionAlive` performs a heartbeat check to determine if the connection is still active. For local connections, this always returns 1 (alive) since local connections are always available as long as the handle is valid.

For remote connections, the function conservatively assumes the connection is alive if the table handle exists. In a full implementation, this would perform an actual network ping or keepalive check.

You can pass a connection handle, table handle, or any derived handle — the function resolves through the handle registry to check the connection status.

## Example

```c
ADSHANDLE hConn;
UNSIGNED16 bAlive;

// Check local connection
AdsConnect60("c:\\data", NULL, NULL, NULL, 0, &hConn);
AdsIsConnectionAlive(hConn, &bAlive);
printf("Local connection alive: %s\n", bAlive ? "yes" : "no");

// Check remote connection
AdsConnect60("tcp://server:6247", NULL, NULL, NULL, 0, &hConn);
AdsIsConnectionAlive(hConn, &bAlive);
printf("Remote connection alive: %s\n", bAlive ? "yes" : "no");

AdsDisconnect(hConn);
```

## See Also

- [AdsConnect60]({{ site.baseurl }}/en/functions/ads-connect60/)
- [AdsGetConnectionType]({{ site.baseurl }}/en/functions/ads-get-connection-type/)

---

[← AdsGetTableOpenOptions]({{ site.baseurl }}/en/functions/ads-get-table-open-options/)
[AdsIsNull →]({{ site.baseurl }}/en/functions/ads-is-null/)
