---
title: AdsCopyTableStructure
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-copy-table-structure/
---

# AdsCopyTableStructure

Cópia apenas a estrutura da tabela.

## Sintaxe

```c
UNSIGNED32 AdsCopyTableStructure(ADSHANDLE hTable, UNSIGNED8* pucFile);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucFile` | `UNSIGNED8*` | Nome do arquivo de destino. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsCopyTableStructure` cria uma nova tabela vazia com a mesma estrutura de campos da tabela original.

## Exemplo

```c
AdsCopyTableStructure(hTable, "nova_tabela.dbf");
// nova_tabela.dbf tem a mesma estrutura mas sem registos
```

## Ver Também

- [AdsCopyTable]({{ site.baseurl }}/pt/funcoes/ads-copy-table/)
- [AdsCopyTableContent]({{ site.baseurl }}/pt/funcoes/ads-copy-table-content/)
- [AdsCreateTable]({{ site.baseurl }}/pt/funcoes/ads-create-table/)

---

[AdsCopyTableContent →]({{ site.baseurl }}/pt/funcoes/ads-copy-table-content/)
