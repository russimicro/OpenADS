---
title: AdsSetSearchPath
layout: default
parent: API Reference
nav_order: 1
permalink: /en/functions/ads-set-search-path/
---

# AdsSetSearchPath

Sets the table search path recorded for the session.

## Syntax

```c
UNSIGNED32 AdsSetSearchPath(UNSIGNED8 *pucPath);
```

## Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pucPath` | `UNSIGNED8*` | Semicolon-separated list of directories. A null or empty string clears it. |

## Return Value

`AE_SUCCESS` (0).

## Description

`AdsSetSearchPath` records a search path that `AdsGetSearchPath` returns. In ADS this is the list of directories searched when opening a table by a bare name. OpenADS resolves paths against the connection's data path, so the value is stored for ACE API parity and round-trips through `AdsGetSearchPath`.

## Example

```c
AdsSetSearchPath((UNSIGNED8 *)"C:\\DATA;C:\\SHARED");
```

## See Also

- [AdsGetSearchPath]({{ site.baseurl }}/en/functions/ads-get-search-path/)
- [AdsSetDefault]({{ site.baseurl }}/en/functions/ads-set-default/)

---

[AdsGetSearchPath →]({{ site.baseurl }}/en/functions/ads-get-search-path/)
