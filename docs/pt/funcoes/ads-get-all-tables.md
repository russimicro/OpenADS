---
title: AdsGetAllTables
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-all-tables/
---

# AdsGetAllTables

Retorna os handles de todas as tabelas abertas.

## Sintaxe

```c
UNSIGNED32 AdsGetAllTables(ADSHANDLE hConnect, ADSHANDLE* ahTable,
                           UNSIGNED16* pusArrayLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `ahTable` | `ADSHANDLE*` | Array para receber os handles das tabelas. |
| `pusArrayLen` | `UNSIGNED16*` | Tamanho do array. Na saída, contém o número de tabelas. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsGetAllTables` retorna os handles de todas as tabelas atualmente abertas para a conexão especificada.

## Exemplo

```c
ADSHANDLE arr[64];
UNSIGNED16 usLen = 64;
AdsGetAllTables(hConnect, arr, &usLen);
// arr contém os handles das tabelas abertas
```

## Ver Também

- [AdsGetNumOpenTables]({{ site.baseurl }}/pt/funcoes/ads-get-num-open-tables/)
- [AdsOpenTable]({{ site.baseurl }}/pt/funcoes/ads-open-table/)
- [AdsCloseTable]({{ site.baseurl }}/pt/funcoes/ads-close-table/)

---

[AdsOpenTable →]({{ site.baseurl }}/pt/funcoes/ads-open-table/)
