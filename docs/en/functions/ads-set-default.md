---
title: AdsSetDefault
layout: default
parent: API Reference
nav_order: 1
permalink: /en/functions/ads-set-default/
---

# AdsSetDefault

Sets the default directory recorded for the session.

## Syntax

```c
UNSIGNED32 AdsSetDefault(UNSIGNED8 *pucPath);
```

## Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pucPath` | `UNSIGNED8*` | Default directory path. A null or empty string clears it. |

## Return Value

`AE_SUCCESS` (0).

## Description

`AdsSetDefault` records a default directory string that `AdsGetDefault` returns. In ADS this is the directory used to resolve relative table names. OpenADS resolves paths against the connection's own data path, so the value is stored for ACE API parity and round-trips through `AdsGetDefault`.

## Example

```c
AdsSetDefault((UNSIGNED8 *)"C:\\DATA\\APP");

UNSIGNED8 buf[260];
UNSIGNED16 len = sizeof(buf);
AdsGetDefault(buf, &len);   // "C:\DATA\APP"
```

## See Also

- [AdsGetDefault]({{ site.baseurl }}/en/functions/ads-get-default/)
- [AdsSetSearchPath]({{ site.baseurl }}/en/functions/ads-set-search-path/)

---

[AdsGetDefault →]({{ site.baseurl }}/en/functions/ads-get-default/)
