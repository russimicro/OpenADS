---
title: AdsSetJulian
layout: default
parent: Referência da API
nav_order: 37
permalink: /pt/funcoes/ads-set-julian/
---

# AdsSetJulian

Define o valor de um campo de data usando número Juliano.

## Sintaxe

```c
UNSIGNED32 AdsSetJulian(ADSHANDLE hTable, UNSIGNED8* pucField,
                        SIGNED32 lJulian);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucField` | `UNSIGNED8*` | Nome ou índice ordinal (1-based) do campo. |
| `lJulian` | `SIGNED32` | Valor Juliano da data. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsSetJulian` define o valor de um campo de data usando um número Juliano. O número Juliano é o número de dias desde 24 de janeiro de 4713 a.C., o que torna cálculos de data mais simples.

## Exemplo

```c
// Define a data para 25 de dezembro de 2025
AdsSetJulian(hTable, "DataEntrega", 2460670);
```

## Ver Também

- [AdsGetJulian]({{ site.baseurl }}/pt/funcoes/ads-get-julian/)
- [AdsSetDateFormat]({{ site.baseurl }}/pt/funcoes/ads-set-date-format/)
- [AdsSetMilliseconds]({{ site.baseurl }}/pt/funcoes/ads-set-milliseconds/)

---

[AdsSetMilliseconds →]({{ site.baseurl }}/pt/funcoes/ads-set-milliseconds/)
