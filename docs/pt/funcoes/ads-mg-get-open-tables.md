---
title: AdsMgGetOpenTables
layout: default
parent: Referência da API
nav_order: 22
permalink: /pt/funcoes/ads-mg-get-open-tables/
---

# AdsMgGetOpenTables

Retorna as tabelas abertas no servidor.

## Sintaxe

```c
UNSIGNED32 AdsMgGetOpenTables(ADSHANDLE hMg, UNSIGNED8* pucUser,
                              UNSIGNED16 usConnNumber, void* pInfo,
                              UNSIGNED16* pusCount, UNSIGNED16* pusSize);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hMg` | `ADSHANDLE` | Handle da conexão de gerenciamento. |
| `pucUser` | `UNSIGNED8*` | Nome do usuário (ou NULL para todos). |
| `usConnNumber` | `UNSIGNED16` | Número da conexão (0 para todas). |
| `pInfo` | `void*` | Array de estruturas `ADS_MGMT_TABLE_INFO` de saída. |
| `pusCount` | `UNSIGNED16*` | Número de tabelas retornadas. |
| `pusSize` | `UNSIGNED16*` | Tamanho total dos dados retornados em bytes. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsMgGetOpenTables` retorna informações sobre todas as tabelas abertas no servidor, filtrando por usuário e/ou número de conexão. Cada entrada inclui o nome da tabela, o usuário proprietário, o número da conexão, o modo de abertura e o tipo de bloqueio.

## Exemplo

```c
ADS_MGMT_TABLE_INFO astTables[100];
memset(astTables, 0, sizeof(astTables));
UNSIGNED16 usCount = 100;
UNSIGNED16 usSize = sizeof(astTables);
AdsMgGetOpenTables(hMgmt, NULL, 0, astTables, &usCount, &usSize);
```

## Ver Também

- [AdsMgGetOpenTables2]({{ site.baseurl }}/pt/funcoes/ads-mg-get-open-tables-2/)
- [AdsMgGetOpenIndexes]({{ site.baseurl }}/pt/funcoes/ads-mg-get-open-indexes/)
- [AdsGetNumOpenTables]({{ site.baseurl }}/pt/funcoes/ads-get-num-open-tables/)

---

[AdsMgGetOpenTables2 →]({{ site.baseurl }}/pt/funcoes/ads-mg-get-open-tables-2/)
