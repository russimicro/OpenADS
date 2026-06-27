---
title: AdsCreateIndex
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-create-index/
---

# AdsCreateIndex

Cria um novo índice.

## Sintaxe

```c
UNSIGNED32 AdsCreateIndex(ADSHANDLE hTable, UNSIGNED8* pucFile,
                          UNSIGNED8* pucTag, UNSIGNED8* pucExpr,
                          UNSIGNED8* pucCondition, UNSIGNED32 ulOptions,
                          UNSIGNED16 usKeyType, ADSHANDLE* phIndex);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucFile` | `UNSIGNED8*` | Nome do arquivo de índice. |
| `pucTag` | `UNSIGNED8*` | Nome do tag. |
| `pucExpr` | `UNSIGNED8*` | Expressão do índice (nome do campo). |
| `pucCondition` | `UNSIGNED8*` | Condição FOR (opcional). |
| `ulOptions` | `UNSIGNED32` | Opções (reservado). |
| `usKeyType` | `UNSIGNED16` | Tipo da chave (reservado). |
| `phIndex` | `ADSHANDLE*` | Ponteiro para receber o handle do índice. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_COLUMN_NOT_FOUND` (6125) se o campo não for encontrado.

## Descrição

`AdsCreateIndex` cria um novo índice para a tabela. A expressão deve ser o nome de um campo existente.

Se uma condição FOR for especificada, apenas os registos que satisfazem a condição são indexados.

## Exemplo

```c
ADSHANDLE hIndex;
AdsCreateIndex(hTable, "dados.cdx", "Nome", "Nome", nullptr, 0, 0, &hIndex);
```

## Ver Também

- [AdsOpenIndex]({{ site.baseurl }}/pt/funcoes/ads-open-index/)
- [AdsCloseIndex]({{ site.baseurl }}/pt/funcoes/ads-close-index/)
- [AdsReindex]({{ site.baseurl }}/pt/funcoes/ads-reindex/)

---

[AdsReindex →]({{ site.baseurl }}/pt/funcoes/ads-reindex/)
