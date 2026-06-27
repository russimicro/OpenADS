---
title: AdsMgGetConfigInfo
layout: default
parent: Referência da API
nav_order: 17
permalink: /pt/funcoes/ads-mg-get-config-info/
---

# AdsMgGetConfigInfo

Retorna informações de configuração do servidor.

## Sintaxe

```c
UNSIGNED32 AdsMgGetConfigInfo(ADSHANDLE hMg, void* pVals,
                              UNSIGNED16* pusValsSize, void* pMem,
                              UNSIGNED16* pusMemSize);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hMg` | `ADSHANDLE` | Handle da conexão de gerenciamento. |
| `pVals` | `void*` | Ponteiro para estrutura `ADS_MGMT_CONFIG_PARAMS` de saída. |
| `pusValsSize` | `UNSIGNED16*` | Tamanho da estrutura de parâmetros (entrada) e bytes escritos (saída). |
| `pMem` | `void*` | Ponteiro para estrutura `ADS_MGMT_CONFIG_MEMORY` de saída. |
| `pusMemSize` | `UNSIGNED16*` | Tamanho da estrutura de memória (entrada) e bytes escritos (saída). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsMgGetConfigInfo` retorna informações de configuração do servidor Advantage, incluindo limites de recursos (conexões, áreas de trabalho, tabelas, etc.) e uso de memória.

## Exemplo

```c
ADS_MGMT_CONFIG_PARAMS stVals;
ADS_MGMT_CONFIG_MEMORY stMem;
memset(&stVals, 0, sizeof(stVals));
memset(&stMem, 0, sizeof(stMem));
UNSIGNED16 usValsSize = sizeof(stVals);
UNSIGNED16 usMemSize = sizeof(stMem);
AdsMgGetConfigInfo(hMgmt, &stVals, &usValsSize, &stMem, &usMemSize);
```

## Ver Também

- [AdsMgGetActivityInfo]({{ site.baseurl }}/pt/funcoes/ads-mg-get-activity-info/)
- [AdsMgGetInstallInfo]({{ site.baseurl }}/pt/funcoes/ads-mg-get-install-info/)
- [AdsMgGetCommStats]({{ site.baseurl }}/pt/funcoes/ads-mg-get-comm-stats/)

---

[AdsMgGetInstallInfo →]({{ site.baseurl }}/pt/funcoes/ads-mg-get-install-info/)
