---
title: AdsRefreshAOF
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-refresh-aof/
---

# AdsRefreshAOF

Atualiza a AOF com base nos dados atuais.

## Sintaxe

```c
UNSIGNED32 AdsRefreshAOF(ADSHANDLE hTable);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsRefreshAOF` reavalia a expressão da AOF contra os dados atuais e reinstala o bitmap, garantindo que linhas alteradas desde `AdsSetAOF` sejam reclassificadas.

Para tabelas remotas, a AOF é mantida no servidor.

## Exemplo

```c
AdsRefreshAOF(hTable);
```

## Ver Também

- [AdsSetAOF]({{ site.baseurl }}/pt/funcoes/ads-set-aof/)
- [AdsGetAOF]({{ site.baseurl }}/pt/funcoes/ads-get-aof/)
- [AdsClearAOF]({{ site.baseurl }}/pt/funcoes/ads-clear-aof/)

---

[AdsCustomizeAOF →]({{ site.baseurl }}/pt/funcoes/ads-customize-aof/)
