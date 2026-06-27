---
title: AdsGetEpoch
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-epoch/
---

# AdsGetEpoch

Retorna o epoch para datas de dois dígitos.

## Sintaxe

```c
UNSIGNED32 AdsGetEpoch(UNSIGNED16* p);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `p` | `UNSIGNED16*` | Ponteiro para receber o ano base do epoch. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsGetEpoch` retorna o ano base para interpretação de datas de dois dígitos.

## Exemplo

```c
UNSIGNED16 usEpoch;
AdsGetEpoch(&usEpoch);
// usEpoch contém o ano base
```

## Ver Também

- [AdsSetEpoch]({{ site.baseurl }}/pt/funcoes/ads-set-epoch/)
- [AdsGetDateFormat]({{ site.baseurl }}/pt/funcoes/ads-get-date-format/)
- [AdsSetDateFormat]({{ site.baseurl }}/pt/funcoes/ads-set-date-format/)

---

[AdsGetSearchPath →]({{ site.baseurl }}/pt/funcoes/ads-get-search-path/)
