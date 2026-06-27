---
title: AdsSetDecimals
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-set-decimals/
---

# AdsSetDecimals

Define o número de casas decimais padrão.

## Sintaxe

```c
UNSIGNED32 AdsSetDecimals(UNSIGNED16 usDecimals);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `usDecimals` | `UNSIGNED16` | Número de casas decimais. |

## Valor de Retorno

`AE_SUCCESS` (0) sempre.

## Descrição

`AdsSetDecimals` define o número de casas decimais padrão para campos numéricos.

## Exemplo

```c
AdsSetDecimals(4);
```

## Ver Também

- [AdsGetFieldDecimals]({{ site.baseurl }}/pt/funcoes/ads-get-field-decimals/)
- [AdsSetDouble]({{ site.baseurl }}/pt/funcoes/ads-set-double/)
- [AdsSetDateFormat]({{ site.baseurl }}/pt/funcoes/ads-set-date-format/)

---

[AdsSetDefault →]({{ site.baseurl }}/pt/funcoes/ads-set-default/)
