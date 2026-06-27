---
title: AdsGetFieldType
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-field-type/
---

# AdsGetFieldType

Retorna o tipo de um campo.

## Sintaxe

```c
UNSIGNED32 AdsGetFieldType(ADSHANDLE hTable, UNSIGNED8* pucField,
                           UNSIGNED16* pusType);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucField` | `UNSIGNED8*` | Nome do campo ou ordinal (via ADSFIELD). |
| `pusType` | `UNSIGNED16*` | Ponteiro para receber o tipo do campo. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_COLUMN_NOT_FOUND` (6125) se o campo não for encontrado.

## Descrição

`AdsGetFieldType` retorna o tipo de um campo especificado. O campo pode ser identificado pelo nome ou pelo ordinal (usando a macro `ADSFIELD(n)`).

Tipos de campo suportados:
- `ADS_STRING` (4) - Caracteres
- `ADS_NUMERIC` (2) - Numérico
- `ADS_LOGICAL` (1) - Lógico
- `ADS_DATE` (3) - Data
- `ADS_TIMESTAMP` (14) - Data/hora
- `ADS_MEMO` (5) - Memo
- `ADS_INTEGER` (11) - Inteiro
- `ADS_MONEY` (18) - Moeda
- `ADS_DOUBLE` (10) - Duplo
- `ADS_SHORTINT` (12) - Inteiro curto
- `ADS_BINARY` (6) - Binário
- `ADS_IMAGE` (7) - Imagem

## Exemplo

```c
UNSIGNED16 usType;
AdsGetFieldType(hTable, "Nome", &usType);
// usType contém o tipo do campo
```

## Ver Também

- [AdsGetFieldName]({{ site.baseurl }}/pt/funcoes/ads-get-field-name/)
- [AdsGetFieldLength]({{ site.baseurl }}/pt/funcoes/ads-get-field-length/)
- [AdsGetFieldDecimals]({{ site.baseurl }}/pt/funcoes/ads-get-field-decimals/)

---

[AdsGetFieldLength →]({{ site.baseurl }}/pt/funcoes/ads-get-field-length/)
