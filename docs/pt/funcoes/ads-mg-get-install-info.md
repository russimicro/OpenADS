---
title: AdsMgGetInstallInfo
layout: default
parent: Referência da API
nav_order: 18
permalink: /pt/funcoes/ads-mg-get-install-info/
---

# AdsMgGetInstallInfo

Retorna informações de instalação do servidor.

## Sintaxe

```c
UNSIGNED32 AdsMgGetInstallInfo(ADSHANDLE hMg, void* pInfo,
                               UNSIGNED16* pusSize);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hMg` | `ADSHANDLE` | Handle da conexão de gerenciamento. |
| `pInfo` | `void*` | Ponteiro para estrutura `ADS_MGMT_INSTALL_INFO` de saída. |
| `pusSize` | `UNSIGNED16*` | Tamanho da estrutura (entrada) e bytes escritos (saída). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsMgGetInstallInfo` retorna informações sobre a instalação do servidor Advantage, incluindo proprietário registrado, versão, data de instalação, conjunto de caracteres, data de expiração da avaliação e número de série.

## Exemplo

```c
ADS_MGMT_INSTALL_INFO stInfo;
memset(&stInfo, 0, sizeof(stInfo));
UNSIGNED16 usSize = sizeof(stInfo);
AdsMgGetInstallInfo(hMgmt, &stInfo, &usSize);
printf("Versão: %s\n", stInfo.aucVersionStr);
```

## Ver Também

- [AdsMgGetActivityInfo]({{ site.baseurl }}/pt/funcoes/ads-mg-get-activity-info/)
- [AdsMgGetConfigInfo]({{ site.baseurl }}/pt/funcoes/ads-mg-get-config-info/)
- [AdsGetVersion]({{ site.baseurl }}/pt/funcoes/ads-get-version/)

---

[AdsMgGetLockOwner →]({{ site.baseurl }}/pt/funcoes/ads-mg-get-lock-owner/)
