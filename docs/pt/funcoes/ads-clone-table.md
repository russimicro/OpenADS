---
title: AdsCloneTable
layout: default
parent: Referência da API
nav_order: 16
permalink: /pt/funcoes/ads-clone-table/
---

# AdsCloneTable

Cria uma cópia da tabela aberta.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsCloneTable(ADSHANDLE hTable, ADSHANDLE* phClone);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela original. |
| `phClone` | `ADSHANDLE*` | Ponteiro para receber o handle da cópia. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsCloneTable` cria uma cópia temporária da tabela aberta, incluindo a estrutura e os dados. A cópia é uma tabela física independente.

## Exemplo

```c
ADSHANDLE hClone;
AdsCloneTable(hTable, &hClone);
```

## Ver Também

- [AdsCopyTable]({{ site.baseurl }}/pt/funcoes/ads-copy-table/)
- [AdsCopyTableStructure]({{ site.baseurl }}/pt/funcoes/ads-copy-table-structure/)

---

[AdsConnect26 →]({{ site.baseurl }}/pt/funcoes/ads-connect-26/)
