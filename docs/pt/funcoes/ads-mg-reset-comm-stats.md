---
title: AdsMgResetCommStats
layout: default
parent: Referência da API
nav_order: 28
permalink: /pt/funcoes/ads-mg-reset-comm-stats/
---

# AdsMgResetCommStats

Reseta as estatísticas de comunicação do servidor.

## Sintaxe

```c
UNSIGNED32 AdsMgResetCommStats(ADSHANDLE hMg);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hMg` | `ADSHANDLE` | Handle da conexão de gerenciamento. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsMgResetCommStats` reseta todas as estatísticas de comunicação do servidor para zero. Isso é útil para medir o tráfego de rede em um período específico.

## Exemplo

```c
AdsMgResetCommStats(hMgmt);
// Coleta de estatísticas começa do zero
```

## Ver Também

- [AdsMgGetCommStats]({{ site.baseurl }}/pt/funcoes/ads-mg-get-comm-stats/)
- [AdsMgGetActivityInfo]({{ site.baseurl }}/pt/funcoes/ads-mg-get-activity-info/)
- [AdsMgGetConfigInfo]({{ site.baseurl }}/pt/funcoes/ads-mg-get-config-info/)

---

[AdsOpenTable90 →]({{ site.baseurl }}/pt/funcoes/ads-open-table-90/)
