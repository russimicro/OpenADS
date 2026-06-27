---
title: AdsCloseAllTables
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-close-all-tables/
---

# AdsCloseAllTables

Fecha todas as tabelas abertas.

## Sintaxe

```c
UNSIGNED32 AdsCloseAllTables(void);
```

## Parâmetros

Nenhum.

## Valor de Retorno

`AE_SUCCESS` (0) sempre.

## Descrição

`AdsCloseAllTables` fecha todas as tabelas atualmente abertas e liberta todos os recursos associados.

## Exemplo

```c
AdsCloseAllTables();
```

## Ver Também

- [AdsCloseTable]({{ site.baseurl }}/pt/funcoes/ads-close-table/)
- [AdsGetAllTables]({{ site.baseurl }}/pt/funcoes/ads-get-all-tables/)
- [AdsGetNumOpenTables]({{ site.baseurl }}/pt/funcoes/ads-get-num-open-tables/)

---

[AdsGetNumOpenTables →]({{ site.baseurl }}/pt/funcoes/ads-get-num-open-tables/)
