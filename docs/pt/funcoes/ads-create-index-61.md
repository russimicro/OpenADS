---
title: AdsCreateIndex61
layout: default
parent: Referência da API
nav_order: 23
permalink: /pt/funcoes/ads-create-index-61/
---

# AdsCreateIndex61

Cria um índice (versão 6.1+).

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsCreateIndex61(ADSHANDLE  hTable,
                                       UNSIGNED8* pucFileName,
                                       UNSIGNED8* pucIndexName,
                                       UNSIGNED8* pucExpr,
                                       UNSIGNED8* pucCondition,
                                       UNSIGNED8* pucKeyFilter,
                                       UNSIGNED32 ulOptions,
                                       UNSIGNED16 usPageSize,
                                       ADSHANDLE* phIndex);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucFileName` | `UNSIGNED8*` | Nome do arquivo do índice. |
| `pucIndexName` | `UNSIGNED8*` | Nome da tag do índice. |
| `pucExpr` | `UNSIGNED8*` | Expressão da chave do índice. |
| `pucCondition` | `UNSIGNED8*` | Condição FOR para filtrar registros indexados. |
| `pucKeyFilter` | `UNSIGNED8*` | Filtro de chave personalizado. |
| `ulOptions` | `UNSIGNED32` | Opções (ADS_UNIQUE, ADS_COMPOUND, etc.). |
| `usPageSize` | `UNSIGNED16` | Tamanho da página do índice. |
| `phIndex` | `ADSHANDLE*` | Ponteiro para receber o handle do índice. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsCreateIndex61` cria um índice com suporte a opções avançadas como chaves compostas, condição FOR, e tamanho de página personalizado.

## Exemplo

```c
ADSHANDLE hIndex;
AdsCreateIndex61(hTable, "indice.adx", "nome", "NOME", NULL, NULL,
                 ADS_COMPOUND, 512, &hIndex);
```

## Ver Também

- [AdsCreateIndex]({{ site.baseurl }}/pt/funcoes/ads-create-index/)
- [AdsCreateIndex90]({{ site.baseurl }}/pt/funcoes/ads-create-index-90/)

---

[AdsCreateIndex90 →]({{ site.baseurl }}/pt/funcoes/ads-create-index-90/)
