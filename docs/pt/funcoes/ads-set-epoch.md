---
title: AdsSetEpoch
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-set-epoch/
---

# AdsSetEpoch

Define o epoch para datas de dois dígitos.

## Sintaxe

```c
UNSIGNED32 AdsSetEpoch(UNSIGNED16 us);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `us` | `UNSIGNED16` | Ano base para o epoch. |

## Valor de Retorno

`AE_SUCCESS` (0) sempre.

## Descrição

`AdsSetEpoch` define o ano base para interpretação de datas de dois dígitos. Por exemplo, se o epoch for 1950, "25" será interpretado como 2025.

## Exemplo

```c
AdsSetEpoch(1950);
```

## Ver Também

- [AdsGetEpoch]({{ site.baseurl }}/pt/funcoes/ads-get-epoch/)
- [AdsSetDateFormat]({{ site.baseurl }}/pt/funcoes/ads-set-date-format/)
- [AdsGetDateFormat]({{ site.baseurl }}/pt/funcoes/ads-get-date-format/)

---

[AdsSetExact →]({{ site.baseurl }}/pt/funcoes/ads-set-exact/)
