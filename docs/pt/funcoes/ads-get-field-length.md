---
title: AdsGetFieldLength
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-field-length/
---

# AdsGetFieldLength

Retorna o comprimento de um campo.

## Sintaxe

```c
UNSIGNED32 AdsGetFieldLength(ADSHANDLE hTable, UNSIGNED8* pucField,
                             UNSIGNED32* pulLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucField` | `UNSIGNED8*` | Nome do campo ou ordinal (via ADSFIELD). |
| `pulLen` | `UNSIGNED32*` | Ponteiro para receber o comprimento do campo. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_COLUMN_NOT_FOUND` (6125) se o campo não for encontrado.

## Descrição

`AdsGetFieldLength` retorna o comprimento de um campo especificado. O comprimento é retornado em caracteres para campos de texto, ou em bytes para campos binários.

Para campos de data/hora, o comprimento retornado é 14 (formato "YYYYMMDDHHMMSS") para facilitar a alocação de buffers.

## Exemplo

```c
UNSIGNED32 ulLen;
AdsGetFieldLength(hTable, "Nome", &ulLen);
// ulLen contém o comprimento do campo
```

## Ver Também

- [AdsGetFieldName]({{ site.baseurl }}/pt/funcoes/ads-get-field-name/)
- [AdsGetFieldType]({{ site.baseurl }}/pt/funcoes/ads-get-field-type/)
- [AdsGetFieldDecimals]({{ site.baseurl }}/pt/funcoes/ads-get-field-decimals/)

---

[AdsGetFieldDecimals →]({{ site.baseurl }}/pt/funcoes/ads-get-field-decimals/)
