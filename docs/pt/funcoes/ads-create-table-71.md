---
title: AdsCreateTable71
layout: default
parent: Referência da API
nav_order: 25
permalink: /pt/funcoes/ads-create-table-71/
---

# AdsCreateTable71

Cria uma tabela (versão 7.1+).

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsCreateTable71(ADSHANDLE hConnect,
                                       UNSIGNED8* pucName,
                                       UNSIGNED8* pucAlias,
                                       UNSIGNED16 usTableType,
                                       UNSIGNED16 usCharType,
                                       UNSIGNED16 usLockType,
                                       UNSIGNED16 usCheckRights,
                                       UNSIGNED16 usMemoSize,
                                       UNSIGNED8* pucFields,
                                       UNSIGNED32 ulOptions,
                                       ADSHANDLE* phTable);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `pucName` | `UNSIGNED8*` | Nome da tabela. |
| `pucAlias` | `UNSIGNED8*` | Alias da tabela. |
| `usTableType` | `UNSIGNED16` | Tipo da tabela (ADS_CDX, ADS_ADT, etc.). |
| `usCharType` | `UNSIGNED16` | Tipo de caracteres (ADS_ANSI, ADS_OEM). |
| `usLockType` | `UNSIGNED16` | Tipo de bloqueio. |
| `usCheckRights` | `UNSIGNED16` | Verificar direitos (1=sim, 0=não). |
| `usMemoSize` | `UNSIGNED16` | Tamanho do bloco de memo. |
| `pucFields` | `UNSIGNED8*` | Definição dos campos. |
| `ulOptions` | `UNSIGNED32` | Opções adicionais. |
| `phTable` | `ADSHANDLE*` | Ponteiro para receber o handle da tabela. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsCreateTable71` cria uma tabela com suporte a opções adicionais introduzidas na versão 7.1.

## Exemplo

```c
ADSHANDLE hTable;
AdsCreateTable71(hConnect, "clientes.adt", "clientes",
                 ADS_ADT, ADS_ANSI, ADS_SHARED, 1, 64,
                 "COD N(5),NOME C(50)", 0, &hTable);
```

## Ver Também

- [AdsCreateTable]({{ site.baseurl }}/pt/funcoes/ads-create-table/)
- [AdsCreateTable90]({{ site.baseurl }}/pt/funcoes/ads-create-table-90/)

---

[AdsCreateTable90 →]({{ site.baseurl }}/pt/funcoes/ads-create-table-90/)
