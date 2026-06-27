---
title: AdsSetFieldRaw
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-set-field-raw/
---

# AdsSetFieldRaw

Define o valor bruto de um campo.

## Sintaxe

```c
UNSIGNED32 AdsSetFieldRaw(ADSHANDLE hTable, UNSIGNED8* pucField,
                          UNSIGNED8* pucBuf, UNSIGNED32 ulLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucField` | `UNSIGNED8*` | Nome do campo ou ordinal. |
| `pucBuf` | `UNSIGNED8*` | Buffer com o valor bruto. |
| `ulLen` | `UNSIGNED32` | Comprimento do valor. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_COLUMN_NOT_FOUND` (6125) se o campo não for encontrado.

## Descrição

`AdsSetFieldRaw` define o valor bruto de um campo. Para gravar, é necessário chamar `AdsWriteRecord`.

## Exemplo

```c
AdsSetFieldRaw(hTable, "Nome", buffer, ulLen);
AdsWriteRecord(hTable);
```

## Ver Também

- [AdsGetFieldRaw]({{ site.baseurl }}/pt/funcoes/ads-get-field-raw/)
- [AdsSetField]({{ site.baseurl }}/pt/funcoes/ads-set-field/)
- [AdsWriteRecord]({{ site.baseurl }}/pt/funcoes/ads-write-record/)

---

[AdsGetIndexExpr →]({{ site.baseurl }}/pt/funcoes/ads-get-index-expr/)
