---
title: AdsOpenTable
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-open-table/
---

# AdsOpenTable

Abre uma tabela existente.

## Sintaxe

```c
UNSIGNED32 AdsOpenTable(ADSHANDLE  hConnect,
                        UNSIGNED8* pucName,
                        UNSIGNED8* pucAlias,
                        UNSIGNED16 usTableType,
                        UNSIGNED16 usCharType,
                        UNSIGNED16 usLockType,
                        UNSIGNED16 usCheckRights,
                        UNSIGNED16 usMode,
                        ADSHANDLE* phTable);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `pucName` | `UNSIGNED8*` | Nome da tabela (caminho completo ou relativo). |
| `pucAlias` | `UNSIGNED8*` | Alias da tabela (reservado). |
| `usTableType` | `UNSIGNED16` | Tipo da tabela: `ADS_CDX`, `ADS_NTX`, `ADS_ADT`. |
| `usCharType` | `UNSIGNED16` | Tipo de caracteres (reservado). |
| `usLockType` | `UNSIGNED16` | Tipo de bloqueio (reservado). |
| `usCheckRights` | `UNSIGNED16` | Verificar direitos (reservado). |
| `usMode` | `UNSIGNED16` | Modo de abertura (reservado). |
| `phTable` | `ADSHANDLE*` | Ponteiro para receber o handle da tabela. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se a tabela não for encontrada.

## Descrição

`AdsOpenTable` abre uma tabela existente e retorna um handle para acesso. Se o nome não tiver extensão, `.dbf` é adicionado por omissão.

Para conexões remotas, a tabela é aberta no servidor.

## Exemplo

```c
ADSHANDLE hTable;
AdsOpenTable(hConnect, "clientes.dbf", nullptr, ADS_CDX, 0, 0, 0, 0, &hTable);
```

## Ver También

- [AdsCreateTable]({{ site.baseurl }}/pt/funcoes/ads-create-table/)
- [AdsCloseTable]({{ site.baseurl }}/pt/funcoes/ads-close-table/)
- [AdsGetTableAlias]({{ site.baseurl }}/pt/funcoes/ads-get-table-alias/)

---

[AdsCloseTable →]({{ site.baseurl }}/pt/funcoes/ads-close-table/)
