---
title: AdsGetDateFormat
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-date-format/
---

# AdsGetDateFormat

Retorna o formato de data ativo.

## Sintaxe

```c
UNSIGNED32 AdsGetDateFormat(UNSIGNED8* pucBuf, UNSIGNED16* pusLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `pucBuf` | `UNSIGNED8*` | Buffer para receber o formato. |
| `pusLen` | `UNSIGNED16*` | Ponteiro para o tamanho do buffer. Na saída, contém o comprimento. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsGetDateFormat` retorna o formato de data ativo (ex: "YYYY-MM-DD").

## Exemplo

```c
UNSIGNED8 szFormat[32];
UNSIGNED16 usLen = sizeof(szFormat);
AdsGetDateFormat(szFormat, &usLen);
// szFormat contém o formato de data
```

## Ver Também

- [AdsSetDateFormat]({{ site.baseurl }}/pt/funcoes/ads-set-date-format/)
- [AdsGetServerTime]({{ site.baseurl }}/pt/funcoes/ads-get-server-time/)
- [AdsGetEpoch]({{ site.baseurl }}/pt/funcoes/ads-get-epoch/)

---

[AdsSetDateFormat →]({{ site.baseurl }}/pt/funcoes/ads-set-date-format/)
