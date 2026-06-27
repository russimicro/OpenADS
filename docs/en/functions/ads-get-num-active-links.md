---
title: AdsGetNumActiveLinks
layout: default
parent: API Reference
nav_order: 1
permalink: /en/functions/ads-get-num-active-links/
---

# AdsGetNumActiveLinks

Returns the count of active remote connections.

## Syntax

```c
UNSIGNED32 AdsGetNumActiveLinks(ADSHANDLE hConnect, UNSIGNED16 *pusNumLinks);
```

## Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `hConnect` | `ADSHANDLE` | Handle of the connection (currently unused in implementation). |
| `pusNumLinks` | `UNSIGNED16*` | Output — number of active remote connections. |

## Return Value

`AE_SUCCESS` (0) on success.

## Description

`AdsGetNumActiveLinks` returns the count of active remote connections currently registered in the handle registry. It iterates through all handles and counts those with `HandleKind::RemoteConnection`.

For local-only applications, this will always return 0. For applications connected to remote servers via TCP/TLS, it returns the number of active remote connections.

Note: The `hConnect` parameter is currently unused in the implementation — the function counts all remote connections regardless of which handle is passed.

## Example

```c
ADSHANDLE hConn1, hConn2;
UNSIGNED16 usNumLinks;

// Connect to remote servers
AdsConnect60("tcp://server1:6247", NULL, NULL, NULL, 0, &hConn1);
AdsConnect60("tcp://server2:6247", NULL, NULL, NULL, 0, &hConn2);

AdsGetNumActiveLinks(0, &usNumLinks);
printf("Active remote connections: %u\n", usNumLinks);  // Output: 2

AdsDisconnect(hConn1);
AdsDisconnect(hConn2);

AdsGetNumActiveLinks(0, &usNumLinks);
printf("After disconnect: %u\n", usNumLinks);  // Output: 0
```

## See Also

- [AdsConnect60]({{ site.baseurl }}/en/functions/ads-connect60/)
- [AdsDisconnect]({{ site.baseurl }}/en/functions/ads-disconnect/)
- [AdsGetConnectionType]({{ site.baseurl }}/en/functions/ads-get-connection-type/)

---

[← AdsGetKeyType]({{ site.baseurl }}/en/functions/ads-get-key-type/)
[AdsGetNumLocks →]({{ site.baseurl }}/en/functions/ads-get-num-locks/)
