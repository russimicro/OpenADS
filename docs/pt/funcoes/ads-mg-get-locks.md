---
title: AdsMgGetLocks
layout: default
parent: Referência da API
nav_order: 20
permalink: /pt/funcoes/ads-mg-get-locks/
---

# AdsMgGetLocks

Retorna os bloqueios mantidos por um usuário ou em uma tabela.

## Sintaxe

```c
UNSIGNED32 AdsMgGetLocks(ADSHANDLE hMg, UNSIGNED8* pucTable,
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
| `pInfo` | `void*` | Array de estruturas `ADS_MGMT_LOCK_INFO` de saída. |
| `pusCount` | `UNSIGNED16*` | Número de bloqueios retornados. |
| `pusSize` | `UNSIGNED16*` | Tamanho total dos dados retornados em bytes. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsMgGetLocks` retorna informações sobre todos os bloqueios de registro mantidos pelos filtros especificados (tabela, usuário, conexão). Pode ser usado para obter todos os bloqueios do servidor passando NULL para os filtros.

## Exemplo

```c
ADS_MGMT_LOCK_INFO astLocks[100];
memset(astLocks, 0, sizeof(astLocks));
UNSIGNED16 usCount = 100;
UNSIGNED16 usSize = sizeof(astLocks);
AdsMgGetLocks(hMgmt, NULL, NULL, 0, astLocks, &usCount, &usSize);
```

## Ver Também

- [AdsMgGetLockOwner]({{ site.baseurl }}/pt/funcoes/ads-mg-get-lock-owner/)
- [AdsGetAllLocks]({{ site.baseurl }}/pt/funcoes/ads-get-all-locks/)
- [AdsIsRecordLocked]({{ site.baseurl }}/pt/funcoes/ads-is-record-locked/)

---

[AdsMgGetOpenIndexes →]({{ site.baseurl }}/pt/funcoes/ads-mg-get-open-indexes/)
