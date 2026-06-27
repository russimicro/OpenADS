---
title: AdsGetFieldDecimals
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-field-decimals/
---

# AdsGetFieldDecimals

Retorna o número de casas decimais de um campo numérico.

## Sintaxe

```c
UNSIGNED32 AdsGetFieldDecimals(ADSHANDLE hTable, UNSIGNED8* pucField,
                               UNSIGNED16* pusDec);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucField` | `UNSIGNED8*` | Nome do campo ou ordinal (via ADSFIELD). |
| `pusDec` | `UNSIGNED16*` | Ponteiro para receber o número de casas decimais. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_COLUMN_NOT_FOUND` (6125) se o campo não for encontrado.

## Descrição

`AdsGetFieldDecimals` retorna o número de casas decimais de um campo numérico. Para campos não numéricos, o valor retornado é 0.

## Exemplo

```c
UNSIGNED16 usDec;
AdsGetFieldDecimals(hTable, "Preco", &usDec);
// usDec contém o número de casas decimais
```

## Ver Também

- [AdsGetFieldName]({{ site.baseurl }}/pt/funcoes/ads-get-field-name/)
- [AdsGetFieldType]({{ site.baseurl }}/pt/funcoes/ads-get-field-type/)
- [AdsGetFieldLength]({{ site.baseurl }}/pt/funcoes/ads-get-field-length/)

---

[AdsGetField →]({{ site.baseurl }}/pt/funcoes/ads-get-field/)
