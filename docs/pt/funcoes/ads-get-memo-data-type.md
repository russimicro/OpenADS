---
title: AdsGetMemoDataType
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-memo-data-type/
---

# AdsGetMemoDataType

Retorna o tipo de dados do campo memo.

## Sintaxe

```c
UNSIGNED32 AdsGetMemoDataType(ADSHANDLE hTable, UNSIGNED8* pucField,
                              UNSIGNED16* pusType);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucField` | `UNSIGNED8*` | Nome do campo memo. |
| `pusType` | `UNSIGNED16*` | Ponteiro para receber o tipo. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_COLUMN_NOT_FOUND` (6125) se o campo não for encontrado.

## Descrição

`AdsGetMemoDataType` retorna o tipo de dados do conteúdo memo:
- `ADS_STRING` (4) - Texto
- `ADS_IMAGE` (7) - Imagem
- `ADS_BINARY` (6) - Objeto binário

## Exemplo

```c
UNSIGNED16 usType;
AdsGetMemoDataType(hTable, "Foto", &usType);
// usType indica o tipo de dados do memo
```

## Ver Também

- [AdsGetMemoLength]({{ site.baseurl }}/pt/funcoes/ads-get-memo-length/)
- [AdsGetFieldType]({{ site.baseurl }}/pt/funcoes/ads-get-field-type/)
- [AdsGetField]({{ site.baseurl }}/pt/funcoes/ads-get-field/)

---

[AdsGetFieldRaw →]({{ site.baseurl }}/pt/funcoes/ads-get-field-raw/)
