---
title: AdsCloseTable
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-close-table/
---

# AdsCloseTable

Fecha uma tabela aberta.

## Sintaxe

```c
UNSIGNED32 AdsCloseTable(ADSHANDLE hTable);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsCloseTable` fecha uma tabela previamente aberta e liberta todos os recursos associados, incluindo índices e relações.

Antes de fechar, a função:
- Faz flush das alterações pendentes para disco
- Remove bindings de índices
- Remove relações
- Invalida projeções de cursor

## Exemplo

```c
AdsCloseTable(hTable);
```

## Ver Também

- [AdsOpenTable]({{ site.baseurl }}/pt/funcoes/ads-open-table/)
- [AdsCloseAllTables]({{ site.baseurl }}/pt/funcoes/ads-close-all-tables/)
- [AdsGetTableAlias]({{ site.baseurl }}/pt/funcoes/ads-get-table-alias/)

---

[AdsCloseAllTables →]({{ site.baseurl }}/pt/funcoes/ads-close-all-tables/)
