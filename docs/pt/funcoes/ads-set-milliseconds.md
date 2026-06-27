---
title: AdsSetMilliseconds
layout: default
parent: Referência da API
nav_order: 38
permalink: /pt/funcoes/ads-set-milliseconds/
---

# AdsSetMilliseconds

Define o valor de um campo de data/hora usando milissegundos.

## Sintaxe

```c
UNSIGNED32 AdsSetMilliseconds(ADSHANDLE hTable, UNSIGNED8* pucField,
                              SIGNED32 lMs);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucField` | `UNSIGNED8*` | Nome ou índice ordinal (1-based) do campo. |
| `lMs` | `SIGNED32` | Número de milissegundos desde midnight. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsSetMilliseconds` define o valor de um campo de data/hora (timestamp) usando o número de milissegundos desde midnight. Útil para medições de tempo de alta precisão.

## Exemplo

```c
// Define para 14:30:00.500 (14h, 30min, 0seg, 500ms)
AdsSetMilliseconds(hTable, "HoraFim", 52200500);
```

## Ver Também

- [AdsGetMilliseconds]({{ site.baseurl }}/pt/funcoes/ads-get-milliseconds/)
- [AdsSetJulian]({{ site.baseurl }}/pt/funcoes/ads-set-julian/)
- [AdsSetTimeStamp]({{ site.baseurl }}/pt/funcoes/ads-set-timestamp/)

---

[AdsSetStringW →]({{ site.baseurl }}/pt/funcoes/ads-set-string-w/)
