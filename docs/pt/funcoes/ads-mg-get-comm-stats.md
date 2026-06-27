---
title: AdsMgGetCommStats
layout: default
parent: Referência da API
nav_order: 16
permalink: /pt/funcoes/ads-mg-get-comm-stats/
---

# AdsMgGetCommStats

Retorna estatísticas de comunicação do servidor.

## Sintaxe

```c
UNSIGNED32 AdsMgGetCommStats(ADSHANDLE hMg, void* pInfo,
                             UNSIGNED16* pusSize);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hMg` | `ADSHANDLE` | Handle da conexão de gerenciamento. |
| `pInfo` | `void*` | Ponteiro para estrutura `ADS_MGMT_COMM_STATS` de saída. |
| `pusSize` | `UNSIGNED16*` | Tamanho da estrutura (entrada) e bytes escritos (saída). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsMgGetCommStats` retorna estatísticas de comunicação do servidor, incluindo percentual de checksums, total de pacotes, pacotes fora de sequência, falhas de checksum e erros de envio/recebimento.

## Exemplo

```c
ADS_MGMT_COMM_STATS stStats;
memset(&stStats, 0, sizeof(stStats));
UNSIGNED16 usSize = sizeof(stStats);
AdsMgGetCommStats(hMgmt, &stStats, &usSize);
printf("Total de pacotes: %lu\n", stStats.ulTotalPackets);
```

## Ver Também

- [AdsMgGetActivityInfo]({{ site.baseurl }}/pt/funcoes/ads-mg-get-activity-info/)
- [AdsMgResetCommStats]({{ site.baseurl }}/pt/funcoes/ads-mg-reset-comm-stats/)
- [AdsMgGetConfigInfo]({{ site.baseurl }}/pt/funcoes/ads-mg-get-config-info/)

---

[AdsMgGetConfigInfo →]({{ site.baseurl }}/pt/funcoes/ads-mg-get-config-info/)
