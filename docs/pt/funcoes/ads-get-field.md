---
title: AdsGetField
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-field/
---

# AdsGetField

Retorna o valor de um campo como texto.

## Sintaxe

```c
UNSIGNED32 AdsGetField(ADSHANDLE hTable, UNSIGNED8* pucField,
                       UNSIGNED8* pucBuf, UNSIGNED32* pulLen,
                       UNSIGNED16 usOption);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucField` | `UNSIGNED8*` | Nome do campo ou ordinal (via ADSFIELD). |
| `pucBuf` | `UNSIGNED8*` | Buffer para receber o valor do campo. |
| `pulLen` | `UNSIGNED32*` | Ponteiro para o tamanho do buffer. Na saída, contém o comprimento do valor. |
| `usOption` | `UNSIGNED16` | Opção reservada (usar 0). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_COLUMN_NOT_FOUND` (6125) se o campo não for encontrado.

## Descrição

`AdsGetField` retorna o valor de um campo como texto. Para campos de caracteres, o valor é preenchido com espaços até ao comprimento declarado do campo.

Para tabelas remotas, o OpenADS serve o valor do cache de linha, resultando em zero RTT por célula após a primeira navegação.

## Exemplo

```c
UNSIGNED8 szValue[256];
UNSIGNED32 ulLen = sizeof(szValue);
AdsGetField(hTable, "Nome", szValue, &ulLen, 0);
// szValue contém o valor do campo
```

## Ver Também

- [AdsGetString]({{ site.baseurl }}/pt/funcoes/ads-get-string/)
- [AdsGetFieldRaw]({{ site.baseurl }}/pt/funcoes/ads-get-field-raw/)
- [AdsGetFieldName]({{ site.baseurl }}/pt/funcoes/ads-get-field-name/)

---

[AdsSetField →]({{ site.baseurl }}/pt/funcoes/ads-set-field/)
