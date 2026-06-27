---
title: AdsRestructureTable
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-restructure-table/
---

# AdsRestructureTable

Modifica a estrutura de uma tabela.

## Sintaxe

```c
UNSIGNED32 AdsRestructureTable(ADSHANDLE   hConnect,
                               UNSIGNED8*  pucTableName,
                               UNSIGNED8*  pucAlias,
                               UNSIGNED16  usFileType,
                               UNSIGNED16  usCharType,
                               UNSIGNED16  usLockType,
                               UNSIGNED16  usCheckRights,
                               UNSIGNED8*  pucAddFields,
                               UNSIGNED8*  pucDeleteFields,
                               UNSIGNED8*  pucChangeFields);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `pucTableName` | `UNSIGNED8*` | Nome da tabela. |
| `pucAlias` | `UNSIGNED8*` | Alias (reservado). |
| `usFileType` | `UNSIGNED16` | Tipo do arquivo (reservado). |
| `usCharType` | `UNSIGNED16` | Tipo de caracteres (reservado). |
| `usLockType` | `UNSIGNED16` | Tipo de bloqueio (reservado). |
| `usCheckRights` | `UNSIGNED16` | Verificar direitos (reservado). |
| `pucAddFields` | `UNSIGNED8*` | Campos a adicionar (formato rddads). |
| `pucDeleteFields` | `UNSIGNED8*` | Campos a remover (`;` separados). |
| `pucChangeFields` | `UNSIGNED8*` | Campos a modificar (formato rddads). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o nome for nulo.

## Descrição

`AdsRestructureTable` modifica a estrutura de uma tabela existente, permitindo adicionar, remover ou alterar campos.

## Exemplo

```c
AdsRestructureTable(hConn, "clientes.dbf", nullptr, 0, 0, 0, 0,
    "Email C 100", "Telefone", "Nome C 80");
```

## Ver Também

- [AdsCreateTable]({{ site.baseurl }}/pt/funcoes/ads-create-table/)
- [AdsCopyTableStructure]({{ site.baseurl }}/pt/funcoes/ads-copy-table-structure/)
- [AdsGetNumFields]({{ site.baseurl }}/pt/funcoes/ads-get-num-fields/)

---

[AdsBeginTransaction →]({{ site.baseurl }}/pt/funcoes/ads-begin-transaction/)
