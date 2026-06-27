---
title: AdsMgGetUserNames
layout: default
parent: Referência da API
nav_order: 25
permalink: /pt/funcoes/ads-mg-get-user-names/
---

# AdsMgGetUserNames

Retorna os nomes dos usuários conectados ao servidor.

## Sintaxe

```c
UNSIGNED32 AdsMgGetUserNames(ADSHANDLE hMg, UNSIGNED8* pucFile,
                             void* pInfo, UNSIGNED16* pusCount,
                             UNSIGNED16* pusSize);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hMg` | `ADSHANDLE` | Handle da conexão de gerenciamento. |
| `pucFile` | `UNSIGNED8*` | Nome da tabela para filtrar (ou NULL para todos). |
| `pInfo` | `void*` | Array de estruturas `ADS_MGMT_USER_INFO` de saída. |
| `pusCount` | `UNSIGNED16*` | Número de usuários retornados. |
| `pusSize` | `UNSIGNED16*` | Tamanho total dos dados retornados em bytes. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsMgGetUserNames` retorna informações sobre os usuários conectados ao servidor. Se um nome de tabela for especificado, apenas os usuários com essa tabela aberta são retornados.

## Exemplo

```c
ADS_MGMT_USER_INFO astUsers[50];
memset(astUsers, 0, sizeof(astUsers));
UNSIGNED16 usCount = 50;
UNSIGNED16 usSize = sizeof(astUsers);
AdsMgGetUserNames(hMgmt, NULL, astUsers, &usCount, &usSize);
```

## Ver Também

- [AdsMgGetActivityInfo]({{ site.baseurl }}/pt/funcoes/ads-mg-get-activity-info/)
- [AdsMgKillUser]({{ site.baseurl }}/pt/funcoes/ads-mg-kill-user/)
- [AdsMgGetLocks]({{ site.baseurl }}/pt/funcoes/ads-mg-get-locks/)

---

[AdsMgGetWorkerThreadActivity →]({{ site.baseurl }}/pt/funcoes/ads-mg-get-worker-thread-activity/)
