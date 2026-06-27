---
title: AdsCopyTable
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-copy-table/
---

# AdsCopyTable

Cópia uma tabela para um novo arquivo.

## Sintaxe

```c
UNSIGNED32 AdsCopyTable(ADSHANDLE   hHandle,
                        UNSIGNED16  usFilterOption,
                        UNSIGNED8*  pucFile);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hHandle` | `ADSHANDLE` | Handle da tabela. |
| `usFilterOption` | `UNSIGNED16` | Opção de filtro (reservada). |
| `pucFile` | `UNSIGNED8*` | Nome do arquivo de destino. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsCopyTable` cria uma cópia da tabela para um novo arquivo. Apenas registos visíveis são copiados (eliminados são pulados).

## Exemplo

```c
AdsCopyTable(hTable, 0, "copia.dbf");
```

## Ver Também

- [AdsCopyTableStructure]({{ site.baseurl }}/pt/funcoes/ads-copy-table-structure/)
- [AdsCopyTableContent]({{ site.baseurl }}/pt/funcoes/ads-copy-table-content/)
- [AdsConvertTable]({{ site.baseurl }}/pt/funcoes/ads-convert-table/)

---

[AdsCopyTableStructure →]({{ site.baseurl }}/pt/funcoes/ads-copy-table-structure/)
