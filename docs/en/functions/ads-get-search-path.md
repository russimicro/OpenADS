---
title: AdsGetSearchPath
layout: default
parent: API Reference
nav_order: 1
permalink: /en/functions/ads-get-search-path/
---

# AdsGetSearchPath

Returns the table search path recorded for the session.

## Syntax

```c
UNSIGNED32 AdsGetSearchPath(UNSIGNED8 *pucPath, UNSIGNED16 *pusLen);
```

## Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pucPath` | `UNSIGNED8*` | Buffer to receive the search-path string. |
| `pusLen` | `UNSIGNED16*` | Input/output — buffer capacity on input, string length on output. |

## Return Value

`AE_SUCCESS` (0).

## Description

`AdsGetSearchPath` returns the search path previously set with `AdsSetSearchPath`, or an empty string if none was set. The two functions form a round-trip.

## Example

```c
UNSIGNED8 buf[512];
UNSIGNED16 len = sizeof(buf);
AdsGetSearchPath(buf, &len);
```

## See Also

- [AdsSetSearchPath]({{ site.baseurl }}/en/functions/ads-set-search-path/)
- [AdsGetDefault]({{ site.baseurl }}/en/functions/ads-get-default/)

---

[← AdsSetSearchPath]({{ site.baseurl }}/en/functions/ads-set-search-path/)
