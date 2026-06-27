---
title: AdsGetMilliseconds
layout: default
parent: Referência da API
nav_order: 7
permalink: /pt/funcoes/ads-get-milliseconds/
---

# AdsGetMilliseconds

Retorna o valor de um campo de data/hora como milissegundos.

## Sintaxe

```c
UNSIGNED32 AdsGetMilliseconds(ADSHANDLE hTable, UNSIGNED8* pucField,
                              SIGNED32* plMs);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucField` | `UNSIGNED8*` | Nome ou índice ordinal (1-based) do campo. |
| `plMs` | `SIGNED32*` | Ponteiro para variável que recebe os milissegundos. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsGetMilliseconds` obtém o valor de um campo de data/hora (timestamp) e o retorna como o número total de milissegundos desde midnight. Útil para medições de tempo de alta precisão.

## Exemplo

```c
SIGNED32 lMs;
AdsGetMilliseconds(hTable, "HoraInicio", &lMs);
// lMs contém os milissegundos desde midnight
```

## Ver Também

- [AdsSetMilliseconds]({{ site.baseurl }}/pt/funcoes/ads-set-milliseconds/)
- [AdsGetJulian]({{ site.baseurl }}/pt/funcoes/ads-get-julian/)
- [AdsGetServerTime]({{ site.baseurl }}/pt/funcoes/ads-get-server-time/)

---

[AdsGetStringW →]({{ site.baseurl }}/pt/funcoes/ads-get-string-w/)
