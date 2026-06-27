---
title: AdsGetJulian
layout: default
parent: Referência da API
nav_order: 6
permalink: /pt/funcoes/ads-get-julian/
---

# AdsGetJulian

Retorna o valor de um campo de data como número Juliano.

## Sintaxe

```c
UNSIGNED32 AdsGetJulian(ADSHANDLE hTable, UNSIGNED8* pucField,
                        SIGNED32* plJulian);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucField` | `UNSIGNED8*` | Nome ou índice ordinal (1-based) do campo. |
| `plJulian` | `SIGNED32*` | Ponteiro para variável que recebe o valor Juliano. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsGetJulian` obtém o valor de um campo de data e o retorna como um número Juliano. O número Juliano é o número de dias desde 24 de janeiro de 4713 a.C., o que torna cálculos de data mais simples.

## Exemplo

```c
SIGNED32 lJulian;
AdsGetJulian(hTable, "DataPedido", &lJulian);
// lJulian contém o equivalente Juliano da data
```

## Ver Também

- [AdsSetJulian]({{ site.baseurl }}/pt/funcoes/ads-set-julian/)
- [AdsGetDate]({{ site.baseurl }}/pt/funcoes/ads-get-date/)
- [AdsGetMilliseconds]({{ site.baseurl }}/pt/funcoes/ads-get-milliseconds/)

---

[AdsGetMilliseconds →]({{ site.baseurl }}/pt/funcoes/ads-get-milliseconds/)
