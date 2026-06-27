---
title: AdsSetServerType
layout: default
parent: API Reference
nav_order: 1
permalink: /en/functions/ads-set-server-type/
---

# AdsSetServerType

Records the preferred server type(s) for subsequent connections.

## Syntax

```c
UNSIGNED32 AdsSetServerType(UNSIGNED16 usServerOptions);
```

## Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `usServerOptions` | `UNSIGNED16` | Bitmask of server types: `ADS_LOCAL_SERVER` (1), `ADS_REMOTE_SERVER` (2), `ADS_AIS_SERVER`. Combine with bitwise OR. |

## Return Value

`AE_SUCCESS` (0).

## Description

`AdsSetServerType` records which server type(s) the application prefers when establishing a connection. In ADS this restricts which server back-ends `AdsConnect` will attempt.

OpenADS serves both local and remote connections regardless of this setting, so the value is stored for ACE API parity and does not block an otherwise-valid connection. Call it before `AdsConnect` if your code expects the ADS semantics; passing a combined mask (local + remote) is always safe.

## Example

```c
// Prefer a remote server, falling back to local.
AdsSetServerType(ADS_REMOTE_SERVER | ADS_LOCAL_SERVER);

ADSHANDLE hConn;
AdsConnect((UNSIGNED8 *)"\\\\server\\share\\data", &hConn);
```

## See Also

- [AdsConnect]({{ site.baseurl }}/en/functions/ads-connect/)
- [AdsGetConnectionType]({{ site.baseurl }}/en/functions/ads-get-connection-type/)

---

[AdsConnect →]({{ site.baseurl }}/en/functions/ads-connect/)
