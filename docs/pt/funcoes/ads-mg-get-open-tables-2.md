---
title: AdsMgGetOpenTables2
layout: default
parent: Referência da API
nav_order: 23
permalink: /pt/funcoes/ads-mg-get-open-tables-2/
---

# AdsMgGetOpenTables2

Retorna as tabelas abertas no servidor (variante estendida).

## Sintaxe

```c
UNSIGNED32 AdsMgGetOpenTables2(ADSHANDLE hMg, UNSIGNED8* pucUser,
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

`AdsMgGetOpenTables2` é uma variante estendida de `AdsMgGetOpenTables` que retorna informações adicionais sobre as tabelas abertas. O comportamento é essencialmente o mesmo, mas a versão 2 pode incluir campos extras na estrutura de saída.

## Exemplo

```c
ADS_MGMT_TABLE_INFO astTables[100];
memset(astTables, 0, sizeof(astTables));
UNSIGNED16 usCount = 100;
UNSIGNED16 usSize = sizeof(astTables);
AdsMgGetOpenTables2(hMgmt, NULL, 0, astTables, &usCount, &usSize);
```

## Ver Também

- [AdsMgGetOpenTables]({{ site.baseurl }}/pt/funcoes/ads-mg-get-open-tables/)
- [AdsMgGetOpenIndexes]({{ site.baseurl }}/pt/funcoes/ads-mg-get-open-indexes/)
- [AdsGetNumOpenTables]({{ site.baseurl }}/pt/funcoes/ads-get-num-open-tables/)

---

[AdsMgGetServerType →]({{ site.baseurl }}/pt/funcoes/ads-mg-get-server-type/)
