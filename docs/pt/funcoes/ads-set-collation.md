---
title: AdsSetCollation
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-set-collation/
---

# AdsSetCollation

Define a collation da conexão.

## Sintaxe

```c
UNSIGNED32 AdsSetCollation(ADSHANDLE hConnect, UNSIGNED8* pucName);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `pucName` | `UNSIGNED8*` | Nome da collation. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INVALID_CONNECTION_HANDLE` se a conexão for inválida.

## Descrição

`AdsSetCollation` define a collation (ordenação de caracteres) para a conexão.

## Exemplo

```c
AdsSetCollation(hConnect, "GENERAL");
```

## Ver Também

- [AdsGetTableCharType]({{ site.baseurl }}/pt/funcoes/ads-get-table-char-type/)
- [AdsSetDateFormat]({{ site.baseurl }}/pt/funcoes/ads-set-date-format/)
- [AdsSetDefault]({{ site.baseurl }}/pt/funcoes/ads-set-default/)

---

[AdsGetTableCharType →]({{ site.baseurl }}/pt/funcoes/ads-get-table-char-type/)
