---
title: AdsMgGetOpenIndexes
layout: default
parent: Referência da API
nav_order: 21
permalink: /pt/funcoes/ads-mg-get-open-indexes/
---

# AdsMgGetOpenIndexes

Retorna os índices abertos em uma tabela.

## Sintaxe

```c
UNSIGNED32 AdsMgGetOpenIndexes(ADSHANDLE hMg, UNSIGNED8* pucTable,
                               UNSIGNED8* pucUser, UNSIGNED16 usConnNumber,
                               void* pInfo, UNSIGNED16* pusCount,
                               UNSIGNED16* pusSize);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hMg` | `ADSHANDLE` | Handle da conexão de gerenciamento. |
| `pucTable` | `UNSIGNED8*` | Nome da tabela (ou NULL para todos). |
| `pucUser` | `UNSIGNED8*` | Nome do usuário (ou NULL para todos). |
| `usConnNumber` | `UNSIGNED16` | Número da conexão (0 para todas). |
| `pInfo` | `void*` | Array de estruturas `ADS_MGMT_INDEX_INFO` de saída. |
| `pusCount` | `UNSIGNED16*` | Número de índices retornados. |
| `pusSize` | `UNSIGNED16*` | Tamanho total dos dados retornados em bytes. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsMgGetOpenIndexes` retorna informações sobre os índices abertos em uma tabela, filtrando por tabela, usuário e/ou número de conexão. Cada entrada inclui o nome do arquivo de índice, a tag e a expressão.

## Exemplo

```c
ADS_MGMT_INDEX_INFO astIdx[50];
memset(astIdx, 0, sizeof(astIdx));
UNSIGNED16 usCount = 50;
UNSIGNED16 usSize = sizeof(astIdx);
AdsMgGetOpenIndexes(hMgmt, "Pedidos", NULL, 0, astIdx, &usCount, &usSize);
```

## Ver Também

- [AdsMgGetOpenTables]({{ site.baseurl }}/pt/funcoes/ads-mg-get-open-tables/)
- [AdsGetNumIndexes]({{ site.baseurl }}/pt/funcoes/ads-get-num-indexes/)
- [AdsGetIndexName]({{ site.baseurl }}/pt/funcoes/ads-get-index-name/)

---

[AdsMgGetOpenTables →]({{ site.baseurl }}/pt/funcoes/ads-mg-get-open-tables/)
