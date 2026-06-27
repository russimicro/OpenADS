---
title: AdsGetFieldRaw
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-field-raw/
---

# AdsGetFieldRaw

Retorna o valor bruto de um campo.

## Sintaxe

```c
UNSIGNED32 AdsGetFieldRaw(ADSHANDLE hTable, UNSIGNED8* pucField,
                          UNSIGNED8* pucBuf, UNSIGNED32* pulLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucField` | `UNSIGNED8*` | Nome do campo ou ordinal. |
| `pucBuf` | `UNSIGNED8*` | Buffer para receber o valor. |
| `pulLen` | `UNSIGNED32*` | Ponteiro para o tamanho do buffer. Na saída, contém o comprimento. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_COLUMN_NOT_FOUND` (6125) se o campo não for encontrado.

## Descrição

`AdsGetFieldRaw` retorna o valor bruto de um campo. Delega para `AdsGetField`.

## Exemplo

```c
UNSIGNED8 szValue[256];
UNSIGNED32 ulLen = sizeof(szValue);
AdsGetFieldRaw(hTable, "Nome", szValue, &ulLen);
```

## Ver Também

- [AdsGetField]({{ site.baseurl }}/pt/funcoes/ads-get-field/)
- [AdsSetFieldRaw]({{ site.baseurl }}/pt/funcoes/ads-set-field-raw/)
- [AdsGetString]({{ site.baseurl }}/pt/funcoes/ads-get-string/)

---

[AdsSetFieldRaw →]({{ site.baseurl }}/pt/funcoes/ads-set-field-raw/)
