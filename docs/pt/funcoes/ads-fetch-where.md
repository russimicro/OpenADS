---
title: AdsFetchWhere
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-fetch-where/
---

# AdsFetchWhere

Busca registos que atendem a uma condição no servidor.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsFetchWhere(ADSHANDLE hTbl, UNSIGNED8* pszExpr, UNSIGNED8* pszCols, UNSIGNED32 ulMaxRows, UNSIGNED32 ulFlags, ADSHANDLE* phResult);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTbl` | `ADSHANDLE` | Handle da tabela. |
| `pszExpr` | `UNSIGNED8*` | Expressão de condição (estilo Clipper FOR). |
| `pszCols` | `UNSIGNED8*` | Lista separada por vírgulas das colunas a retornar. NULL para modo de contagem. |
| `ulMaxRows` | `UNSIGNED32` | Número máximo de linhas a retornar. |
| `ulFlags` | `UNSIGNED32` | Flags de opção (0x01 = WANT_RECNO para incluir números de registo). |
| `phResult` | `ADSHANDLE*` | Ponteiro para receber o handle do resultado. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_FUNCTION_NOT_AVAILABLE` (5004) para tabelas locais.

## Descrição

`AdsFetchWhere` envia uma expressão de condição ao servidor e recebe um lote de registos correspondentes. O handle do resultado é válido até que AdsFetchWhereClose seja chamado. Os índices de linha são base 0.

Esta função só está disponível para tabelas remotas; tabelas locais retornam `AE_FUNCTION_NOT_AVAILABLE`.

## Exemplo

```c
ADSHANDLE hResult;
UNSIGNED32 ulRows;

AdsFetchWhere(hTable, "Estado = 'AT'", "Nome,Email", 100, 0x01, &hResult);
AdsFetchWhereRows(hResult, &ulRows);
// Processar resultados...
AdsFetchWhereClose(hResult);
```

## Ver Também

- [AdsFetchWhereClose]({{ site.baseurl }}/pt/funcoes/ads-fetch-where-close/)
- [AdsFetchWhereRows]({{ site.baseurl }}/pt/funcoes/ads-fetch-where-rows/)
- [AdsFetchWhereEof]({{ site.baseurl }}/pt/funcoes/ads-fetch-where-eof/)

---

[AdsFetchWhereApplyRow →]({{ site.baseurl }}/pt/funcoes/ads-fetch-where-apply-row/)
