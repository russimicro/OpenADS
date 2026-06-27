---
title: AdsGetDefault
layout: default
parent: API Reference
nav_order: 1
permalink: /en/functions/ads-get-default/
---

# AdsGetDefault

Returns the default directory recorded for the session.

## Syntax

```c
UNSIGNED32 AdsGetDefault(UNSIGNED8 *pucPath, UNSIGNED16 *pusLen);
```

## Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pucPath` | `UNSIGNED8*` | Buffer to receive the default directory string. |
| `pusLen` | `UNSIGNED16*` | Input/output — buffer capacity on input, string length on output. |

## Return Value

`AE_SUCCESS` (0).

## Description

`AdsGetDefault` returns the directory string previously set with `AdsSetDefault`, or an empty string if none was set. The two functions form a round-trip; see `AdsSetDefault` for how OpenADS treats the value.

## Example

```c
UNSIGNED8 buf[260];
UNSIGNED16 len = sizeof(buf);
AdsGetDefault(buf, &len);
```

## See Also

- [AdsSetDefault]({{ site.baseurl }}/en/functions/ads-set-default/)
- [AdsGetSearchPath]({{ site.baseurl }}/en/functions/ads-get-search-path/)

---

[← AdsSetDefault]({{ site.baseurl }}/en/functions/ads-set-default/)
