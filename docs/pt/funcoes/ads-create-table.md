---
title: AdsCreateTable
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-create-table/
---

# AdsCreateTable

Cria uma nova tabela.

## Sintaxe

```c
UNSIGNED32 AdsCreateTable(ADSHANDLE     hConn,
                          UNSIGNED8*    pucName,
                          UNSIGNED8*    pucAlias,
                          UNSIGNED16    usTableType,
                          UNSIGNED16    usCharType,
                          UNSIGNED16    usLockType,
                          UNSIGNED16    usCheckRights,
                          UNSIGNED16    usMemoBlockSize,
                          UNSIGNED8*    pucFields,
                          ADSHANDLE*    phTable);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConn` | `ADSHANDLE` | Handle da conexão. |
| `pucName` | `UNSIGNED8*` | Nome da nova tabela. |
| `pucAlias` | `UNSIGNED8*` | Alias da tabela (reservado). |
| `usTableType` | `UNSIGNED16` | Tipo da tabela: `ADS_CDX`, `ADS_NTX`, `ADS_ADT`. |
| `usCharType` | `UNSIGNED16` | Tipo de caracteres (reservado). |
| `usLockType` | `UNSIGNED16` | Tipo de bloqueio (reservado). |
| `usCheckRights` | `UNSIGNED16` | Verificar direitos (reservado). |
| `usMemoBlockSize` | `UNSIGNED16` | Tamanho do bloco memo. |
| `pucFields` | `UNSIGNED8*` | Definições de campos no formato rddads. |
| `phTable` | `ADSHANDLE*` | Ponteiro para receber o handle da tabela. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se os parâmetros forem inválidos.

## Descrição

`AdsCreateTable` cria uma nova tabela com a estrutura especificada. Se o nome não tiver extensão, `.dbf` ou `.adt` é adicionado conforme o tipo.

## Exemplo

```c
ADSHANDLE hTable;
AdsCreateTable(hConn, "clientes", nullptr, ADS_CDX, 0, 0, 0, 0,
    "Nome C 50;Idade N 3;Ativo L", &hTable);
```

## Ver Também

- [AdsOpenTable]({{ site.baseurl }}/pt/funcoes/ads-open-table/)
- [AdsCloseTable]({{ site.baseurl }}/pt/funcoes/ads-close-table/)
- [AdsCopyTableStructure]({{ site.baseurl }}/pt/funcoes/ads-copy-table-structure/)

---

[AdsCreateIndex →]({{ site.baseurl }}/pt/funcoes/ads-create-index/)
