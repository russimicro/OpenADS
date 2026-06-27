---
title: AdsMgGetLockOwner
layout: default
parent: Referência da API
nav_order: 19
permalink: /pt/funcoes/ads-mg-get-lock-owner/
---

# AdsMgGetLockOwner

Retorna o proprietário de um bloqueio específico.

## Sintaxe

```c
UNSIGNED32 AdsMgGetLockOwner(ADSHANDLE hMg, UNSIGNED8* pucTable,
                             UNSIGNED32 ulRecord, void* pInfo,
                             UNSIGNED16* pusSize, UNSIGNED16* pusLockType);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hMg` | `ADSHANDLE` | Handle da conexão de gerenciamento. |
| `pucTable` | `UNSIGNED8*` | Nome da tabela. |
| `ulRecord` | `UNSIGNED32` | Número do registro bloqueado. |
| `pInfo` | `void*` | Ponteiro para estrutura `ADS_MGMT_LOCK_INFO` de saída. |
| `pusSize` | `UNSIGNED16*` | Tamanho da estrutura (entrada) e bytes escritos (saída). |
| `pusLockType` | `UNSIGNED16*` | Tipo de bloqueio retornado. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsMgGetLockOwner` retorna informações sobre o usuário que possui o bloqueio de um registro específico em uma tabela. Útil para diagnóstico de conflitos de bloqueio.

## Exemplo

```c
ADS_MGMT_LOCK_INFO stInfo;
memset(&stInfo, 0, sizeof(stInfo));
UNSIGNED16 usSize = sizeof(stInfo);
UNSIGNED16 usLockType;
AdsMgGetLockOwner(hMgmt, "Pedidos", 42, &stInfo, &usSize, &usLockType);
printf("Bloqueado por: %s\n", stInfo.aucUserName);
```

## Ver Também

- [AdsMgGetLocks]({{ site.baseurl }}/pt/funcoes/ads-mg-get-locks/)
- [AdsIsRecordLocked]({{ site.baseurl }}/pt/funcoes/ads-is-record-locked/)
- [AdsLockRecord]({{ site.baseurl }}/pt/funcoes/ads-lock-record/)

---

[AdsMgGetLocks →]({{ site.baseurl }}/pt/funcoes/ads-mg-get-locks/)
