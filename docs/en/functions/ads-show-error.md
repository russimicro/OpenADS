---
title: AdsShowError
layout: default
parent: API Reference
nav_order: 1
permalink: /en/functions/ads-show-error/
---

# AdsShowError

Reports an error message to the host.

## Syntax

```c
UNSIGNED32 AdsShowError(UNSIGNED8 *pucErrText);
```

## Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pucErrText` | `UNSIGNED8*` | Null-terminated message to report. A null or empty string is ignored. |

## Return Value

`AE_SUCCESS` (0).

## Description

`AdsShowError` reports an error message to the host. On a GUI host ADS displays a message box; OpenADS is headless, so the closest faithful behaviour is to write the message to standard error (`stderr`), followed by a newline. A null or empty message produces no output. The call always succeeds.

## Example

```c
UNSIGNED8 buf[256];
UNSIGNED16 len = sizeof(buf);
AdsGetLastError(&ulCode, buf, &len);
AdsShowError(buf);
```

## See Also

- [AdsGetLastError]({{ site.baseurl }}/en/functions/ads-get-last-error/)
- [AdsGetErrorString]({{ site.baseurl }}/en/functions/ads-get-error-string/)

---

[AdsGetLastError →]({{ site.baseurl }}/en/functions/ads-get-last-error/)
