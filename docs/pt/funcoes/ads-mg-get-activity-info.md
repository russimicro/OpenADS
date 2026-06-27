---
title: AdsMgGetActivityInfo
layout: default
parent: Referência da API
nav_order: 15
permalink: /pt/funcoes/ads-mg-get-activity-info/
---

# AdsMgGetActivityInfo

Retorna informações de atividade do servidor.

## Sintaxe

```c
UNSIGNED32 AdsMgGetActivityInfo(ADSHANDLE hMg, void* pInfo,
                                UNSIGNED16* pusSize);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hMg` | `ADSHANDLE` | Handle da conexão de gerenciamento. |
| `pInfo` | `void*` | Ponteiro para estrutura `ADS_MGMT_ACTIVITY_INFO` de saída. |
| `pusSize` | `UNSIGNED16*` | Tamanho da estrutura (entrada) e bytes escritos (saída). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsMgGetActivityInfo` retorna informações sobre a atividade atual do servidor, incluindo número de operações, erros registrados, tempo de atividade e uso de recursos (usuários, conexões, tabelas, etc.).

## Exemplo

```c
ADS_MGMT_ACTIVITY_INFO stInfo;
memset(&stInfo, 0, sizeof(stInfo));
UNSIGNED16 usSize = sizeof(stInfo);
AdsMgGetActivityInfo(hMgmt, &stInfo, &usSize);
printf("Operações: %lu\n", stInfo.ulOperations);
```

## Ver Também

- [AdsMgConnect]({{ site.baseurl }}/pt/funcoes/ads-mg-connect/)
- [AdsMgGetCommStats]({{ site.baseurl }}/pt/funcoes/ads-mg-get-comm-stats/)
- [AdsMgGetConfigInfo]({{ site.baseurl }}/pt/funcoes/ads-mg-get-config-info/)

---

[AdsMgGetCommStats →]({{ site.baseurl }}/pt/funcoes/ads-mg-get-comm-stats/)
