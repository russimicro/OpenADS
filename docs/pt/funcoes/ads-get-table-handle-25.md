---
title: AdsGetTableHandle25
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-table-handle-25/
---

# AdsGetTableHandle25

Retorna o handle de uma tabela pelo nome (versão 2.5).

## Sintaxe

```c
UNSIGNED32 AdsGetTableHandle25(ADSHANDLE hConnect, UNSIGNED8* pucName,
                               ADSHANDLE* phTable);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `pucName` | `UNSIGNED8*` | Nome da tabela. |
| `phTable` | `ADSHANDLE*` | Ponteiro para receber o handle. |

## Valor de Retorno

`AE_TABLE_NOT_FOUND` sempre (não implementado).

## Descrição

`AdsGetTableHandle25` tenta retornar o handle de uma tabela pelo nome. No OpenADS, esta função não está implementada e retorna sempre `AE_TABLE_NOT_FOUND`.

## Ver Também

- [AdsOpenTable]({{ site.baseurl }}/pt/funcoes/ads-open-table/)
- [AdsGetAllTables]({{ site.baseurl }}/pt/funcoes/ads-get-all-tables/)
- [AdsFindFirstTable]({{ site.baseurl }}/pt/funcoes/ads-find-first-table/)

---

[AdsGetBookmark60 →]({{ site.baseurl }}/pt/funcoes/ads-get-bookmark-60/)
