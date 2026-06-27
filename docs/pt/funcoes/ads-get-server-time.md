---
title: AdsGetServerTime
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-server-time/
---

# AdsGetServerTime

Retorna a hora do servidor.

## Sintaxe

```c
UNSIGNED32 AdsGetServerTime(ADSHANDLE  hConnect,
                            UNSIGNED8* pucDateBuf, UNSIGNED16* pusDateLen,
                            SIGNED32*  plTime,
                            UNSIGNED8* pucTimeBuf, UNSIGNED16* pusTimeLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `pucDateBuf` | `UNSIGNED8*` | Buffer para receber a data. |
| `pusDateLen` | `UNSIGNED16*` | Ponteiro para o tamanho do buffer de data. |
| `plTime` | `SIGNED32*` | Ponteiro para receber os milissegundos desde a meia-noite. |
| `pucTimeBuf` | `UNSIGNED8*` | Buffer para receber a hora. |
| `pusTimeLen` | `UNSIGNED16*` | Ponteiro para o tamanho do buffer de hora. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se houver erro.

## Descrição

`AdsGetServerTime` retorna a data e hora atuais do servidor.

## Exemplo

```c
UNSIGNED8 szDate[32], szTime[32];
UNSIGNED16 usDateLen = sizeof(szDate), usTimeLen = sizeof(szTime);
SIGNED32 lMs;
AdsGetServerTime(hConnect, szDate, &usDateLen, &lMs, szTime, &usTimeLen);
```

## Ver Também

- [AdsGetServerName]({{ site.baseurl }}/pt/funcoes/ads-get-server-name/)
- [AdsGetDateFormat]({{ site.baseurl }}/pt/funcoes/ads-get-date-format/)
- [AdsSetDateFormat]({{ site.baseurl }}/pt/funcoes/ads-set-date-format/)

---

[AdsGetDateFormat →]({{ site.baseurl }}/pt/funcoes/ads-get-date-format/)
