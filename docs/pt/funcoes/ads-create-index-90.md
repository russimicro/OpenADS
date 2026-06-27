---
title: AdsCreateIndex90
layout: default
parent: Referência da API
nav_order: 24
permalink: /pt/funcoes/ads-create-index-90/
---

# AdsCreateIndex90

Cria um índice (versão 9.0+).

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsCreateIndex90(ADSHANDLE hObj,
                                       UNSIGNED8* pucFileName,
                                       UNSIGNED8* pucTag,
                                       UNSIGNED8* pucExpr,
                                       UNSIGNED8* pucCondition,
                                       UNSIGNED8* pucWhile,
                                       UNSIGNED32 ulOptions,
                                       UNSIGNED32 ulPageSize,
                                       UNSIGNED8* pucCollation,
                                       ADSHANDLE* phIndex);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hObj` | `ADSHANDLE` | Handle da tabela. |
| `pucFileName` | `UNSIGNED8*` | Nome do arquivo do índice. |
| `pucTag` | `UNSIGNED8*` | Nome da tag do índice. |
| `pucExpr` | `UNSIGNED8*` | Expressão da chave do índice. |
| `pucCondition` | `UNSIGNED8*` | Condição FOR. |
| `pucWhile` | `UNSIGNED8*` | Condição WHILE. |
| `ulOptions` | `UNSIGNED32` | Opções (ADS_UNIQUE, ADS_COMPOUND, etc.). |
| `ulPageSize` | `UNSIGNED32` | Tamanho da página do índice. |
| `pucCollation` | `UNSIGNED8*` | Collation para ordenação. |
| `phIndex` | `ADSHANDLE*` | Ponteiro para receber o handle do índice. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsCreateIndex90` é a versão estendida de `AdsCreateIndex61`, adicionando suporte a collation e condição WHILE.

## Exemplo

```c
ADSHANDLE hIndex;
AdsCreateIndex90(hTable, "indice.adx", "nome", "NOME", NULL, NULL,
                 ADS_COMPOUND, 512, NULL, &hIndex);
```

## Ver Também

- [AdsCreateIndex]({{ site.baseurl }}/pt/funcoes/ads-create-index/)
- [AdsCreateIndex61]({{ site.baseurl }}/pt/funcoes/ads-create-index-61/)

---

[AdsCreateTable71 →]({{ site.baseurl }}/pt/funcoes/ads-create-table-71/)
