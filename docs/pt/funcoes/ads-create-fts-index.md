---
title: AdsCreateFTSIndex
layout: default
parent: Referência da API
nav_order: 22
permalink: /pt/funcoes/ads-create-fts-index/
---

# AdsCreateFTSIndex

Cria um índice de pesquisa de texto completo (FTS).

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsCreateFTSIndex(ADSHANDLE   hTable,
                                         UNSIGNED8*  pucFileName,
                                         UNSIGNED8*  pucTag,
                                         UNSIGNED8*  pucField,
                                         UNSIGNED32  ulPageSize,
                                         UNSIGNED32  ulMinWordLen,
                                         UNSIGNED32  ulMaxWordLen,
                                         UNSIGNED16  usUseDefaultDelim,
                                         UNSIGNED8*  pucDelimiters,
                                         UNSIGNED16  usUseDefaultNoise,
                                         UNSIGNED8*  pucNoiseWords,
                                         UNSIGNED16  usUseDefaultDrop,
                                         UNSIGNED8*  pucDropChars,
                                         UNSIGNED16  usUseDefaultConditionals,
                                         UNSIGNED8*  pucConditionalChars,
                                         UNSIGNED8*  pucReserved1,
                                         UNSIGNED8*  pucReserved2,
                                         UNSIGNED32  ulOptions);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucFileName` | `UNSIGNED8*` | Nome do arquivo do índice FTS. |
| `pucTag` | `UNSIGNED8*` | Nome da tag do índice. |
| `pucField` | `UNSIGNED8*` | Nome do campo de texto. |
| `ulPageSize` | `UNSIGNED32` | Tamanho da página do índice. |
| `ulMinWordLen` | `UNSIGNED32` | Comprimento mínimo da palavra. |
| `ulMaxWordLen` | `UNSIGNED32` | Comprimento máximo da palavra. |
| `usUseDefaultDelim` | `UNSIGNED16` | Usar delimitadores padrão (1=sim, 0=não). |
| `pucDelimiters` | `UNSIGNED8*` | Delimitadores personalizados. |
| `usUseDefaultNoise` | `UNSIGNED16` | Usar palavras de ruído padrão (1=sim, 0=não). |
| `pucNoiseWords` | `UNSIGNED8*` | Palavras de ruído personalizadas. |
| `usUseDefaultDrop` | `UNSIGNED16` | Usar caracteres de remoção padrão (1=sim, 0=não). |
| `pucDropChars` | `UNSIGNED8*` | Caracteres de remoção personalizados. |
| `usUseDefaultConditionals` | `UNSIGNED16` | Usar caracteres condicionais padrão (1=sim, 0=não). |
| `pucConditionalChars` | `UNSIGNED8*` | Caracteres condicionais personalizados. |
| `pucReserved1` | `UNSIGNED8*` | Reservado. |
| `pucReserved2` | `UNSIGNED8*` | Reservado. |
| `ulOptions` | `UNSIGNED32` | Opções adicionais. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsCreateFTSIndex` cria um índice de pesquisa de texto completo (Full-Text Search) para um campo de texto da tabela.

## Exemplo

```c
AdsCreateFTSIndex(hTable, "indice.adi", "fts_nome", "NOME",
                  512, 3, 30, 1, NULL, 1, NULL, 1, NULL, 1, NULL,
                  NULL, NULL, 0);
```

## Ver Também

- [AdsFTSSearch]({{ site.baseurl }}/pt/funcoes/ads-fts-search/)

---

[AdsCreateIndex61 →]({{ site.baseurl }}/pt/funcoes/ads-create-index-61/)
