---
title: AdsFetchWhereRows
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-fetch-where-rows/
---

# AdsFetchWhereRows

Retorna o número de linhas no resultado.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsFetchWhereRows(ADSHANDLE hRes, UNSIGNED32* pulRows);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hRes` | `ADSHANDLE` | Handle do resultado. |
| `pulRows` | `UNSIGNED32*` | Ponteiro para receber o número de linhas. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsFetchWhereRows` retorna o número total de linhas no conjunto de resultados retornado por AdsFetchWhere.

## Exemplo

```c
UNSIGNED32 ulRows;

AdsFetchWhere(hTable, "Estado = 'AT'", "Nome", 100, 0, &hResult);
AdsFetchWhereRows(hResult, &ulRows);
printf("Total de registros: %u\n", ulRows);
```

## Ver Também

- [AdsFetchWhere]({{ site.baseurl }}/pt/funcoes/ads-fetch-where/)
- [AdsFetchWhereClose]({{ site.baseurl }}/pt/funcoes/ads-fetch-where-close/)

---

[AdsFileToBinary →]({{ site.baseurl }}/pt/funcoes/ads-file-to-binary/)
