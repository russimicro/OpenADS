---
title: AdsSetDateFormat
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-set-date-format/
---

# AdsSetDateFormat

Define o formato de data.

## Sintaxe

```c
UNSIGNED32 AdsSetDateFormat(UNSIGNED8* pucFormat);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `pucFormat` | `UNSIGNED8*` | Formato de data (ex: "YYYY-MM-DD"). |

## Valor de Retorno

`AE_SUCCESS` (0) sempre.

## Descrição

`AdsSetDateFormat` define o formato de data global para a sessão.

## Exemplo

```c
AdsSetDateFormat("DD/MM/YYYY");
```

## Ver Também

- [AdsGetDateFormat]({{ site.baseurl }}/pt/funcoes/ads-get-date-format/)
- [AdsGetServerTime]({{ site.baseurl }}/pt/funcoes/ads-get-server-time/)
- [AdsSetEpoch]({{ site.baseurl }}/pt/funcoes/ads-set-epoch/)

---

[AdsSetDecimals →]({{ site.baseurl }}/pt/funcoes/ads-set-decimals/)
