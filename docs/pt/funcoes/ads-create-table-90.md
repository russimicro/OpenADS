---
title: AdsCreateTable90
layout: default
parent: Referência da API
nav_order: 26
permalink: /pt/funcoes/ads-create-table-90/
---

# AdsCreateTable90

Cria uma tabela (versão 9.0+).

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsCreateTable90(ADSHANDLE hConnect,
                                       UNSIGNED8* pucName,
                                       UNSIGNED8* pucAlias,
                                       UNSIGNED16 usTableType,
                                       UNSIGNED16 usCharType,
                                       UNSIGNED16 usLockType,
                                       UNSIGNED16 usCheckRights,
                                       UNSIGNED16 usMemoSize,
                                       UNSIGNED8* pucFields,
                                       UNSIGNED32 ulOptions,
                                       UNSIGNED8* pucCollation,
                                       ADSHANDLE* phTable);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `pucName` | `UNSIGNED8*` | Nome da tabela. |
| `pucAlias` | `UNSIGNED8*` | Alias da tabela. |
| `usTableType` | `UNSIGNED16` | Tipo da tabela. |
| `usCharType` | `UNSIGNED16` | Tipo de caracteres. |
| `usLockType` | `UNSIGNED16` | Tipo de bloqueio. |
| `usCheckRights` | `UNSIGNED16` | Verificar direitos. |
| `usMemoSize` | `UNSIGNED16` | Tamanho do bloco de memo. |
| `pucFields` | `UNSIGNED8*` | Definição dos campos. |
| `ulOptions` | `UNSIGNED32` | Opções adicionais. |
| `pucCollation` | `UNSIGNED8*` | Collation para a tabela. |
| `phTable` | `ADSHANDLE*` | Ponteiro para receber o handle da tabela. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsCreateTable90` é a versão estendida de `AdsCreateTable71`, adicionando suporte a collation.

## Exemplo

```c
ADSHANDLE hTable;
AdsCreateTable90(hConnect, "clientes.adt", "clientes",
                 ADS_ADT, ADS_ANSI, ADS_SHARED, 1, 64,
                 "COD N(5),NOME C(50)", 0, NULL, &hTable);
```

## Ver Também

- [AdsCreateTable]({{ site.baseurl }}/pt/funcoes/ads-create-table/)
- [AdsCreateTable71]({{ site.baseurl }}/pt/funcoes/ads-create-table-71/)

---

[AdsDDAddIndexFile →]({{ site.baseurl }}/pt/funcoes/ads-dd-add-index-file/)
