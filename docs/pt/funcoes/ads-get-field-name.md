---
title: AdsGetFieldName
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-field-name/
---

# AdsGetFieldName

Retorna o nome de um campo pelo seu índice.

## Sintaxe

```c
UNSIGNED32 AdsGetFieldName(ADSHANDLE hTable, UNSIGNED16 usFieldNum,
                           UNSIGNED8* pucBuf, UNSIGNED16* pusLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `usFieldNum` | `UNSIGNED16` | Índice do campo (1-based). |
| `pucBuf` | `UNSIGNED8*` | Buffer para receber o nome do campo. |
| `pusLen` | `UNSIGNED16*` | Ponteiro para o tamanho do buffer. Na saída, contém o comprimento do nome. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_COLUMN_NOT_FOUND` (6125) se o índice do campo for inválido.

## Descrição

`AdsGetFieldName` retorna o nome de um campo especificado pelo seu índice. O índice é baseado em 1 (o primeiro campo tem índice 1).

Para tabelas remotas, a descrição da tabela é cacheada após a primeira chamada.

## Exemplo

```c
UNSIGNED8 szName[128];
UNSIGNED16 usLen = sizeof(szName);
AdsGetFieldName(hTable, 1, szName, &usLen);
// szName contém o nome do primeiro campo
```

## Ver Também

- [AdsGetNumFields]({{ site.baseurl }}/pt/funcoes/ads-get-num-fields/)
- [AdsGetFieldType]({{ site.baseurl }}/pt/funcoes/ads-get-field-type/)
- [AdsGetFieldLength]({{ site.baseurl }}/pt/funcoes/ads-get-field-length/)

---

[AdsGetFieldType →]({{ site.baseurl }}/pt/funcoes/ads-get-field-type/)
