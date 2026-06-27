---
title: AdsMgGetWorkerThreadActivity
layout: default
parent: Referência da API
nav_order: 26
permalink: /pt/funcoes/ads-mg-get-worker-thread-activity/
---

# AdsMgGetWorkerThreadActivity

Retorna a atividade das threads de trabalho do servidor.

## Sintaxe

```c
UNSIGNED32 AdsMgGetWorkerThreadActivity(ADSHANDLE hMg, void* pInfo,
                                        UNSIGNED16* pusCount,
                                        UNSIGNED16* pusSize);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hMg` | `ADSHANDLE` | Handle da conexão de gerenciamento. |
| `pInfo` | `void*` | Array de estruturas `ADS_MGMT_THREAD_ACTIVITY` de saída. |
| `pusCount` | `UNSIGNED16*` | Número de threads retornadas. |
| `pusSize` | `UNSIGNED16*` | Tamanho total dos dados retornados em bytes. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsMgGetWorkerThreadActivity` retorna informações sobre a atividade das threads de trabalho do servidor. Cada entrada inclui o número da thread, código da operação, nome do usuário e número da conexão.

## Exemplo

```c
ADS_MGMT_THREAD_ACTIVITY astThreads[20];
memset(astThreads, 0, sizeof(astThreads));
UNSIGNED16 usCount = 20;
UNSIGNED16 usSize = sizeof(astThreads);
AdsMgGetWorkerThreadActivity(hMgmt, astThreads, &usCount, &usSize);
```

## Ver Também

- [AdsMgGetActivityInfo]({{ site.baseurl }}/pt/funcoes/ads-mg-get-activity-info/)
- [AdsMgGetConfigInfo]({{ site.baseurl }}/pt/funcoes/ads-mg-get-config-info/)
- [AdsMgGetCommStats]({{ site.baseurl }}/pt/funcoes/ads-mg-get-comm-stats/)

---

[AdsMgKillUser →]({{ site.baseurl }}/pt/funcoes/ads-mg-kill-user/)
