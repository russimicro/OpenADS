---
title: AdsResetConnection
layout: default
parent: Referência da API
nav_order: 34
permalink: /pt/funcoes/ads-reset-connection/
---

# AdsResetConnection

Reseta o estado de uma conexão.

## Sintaxe

```c
UNSIGNED32 AdsResetConnection(ADSHANDLE hConnect);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsResetConnection` reseta o estado de uma conexão, fechando todas as tabelas, índices e cursores associados. A conexão em si permanece aberta e pode ser reutilizada.

## Exemplo

```c
AdsResetConnection(hConnect);
// Todas as tabelas e cursores são fechados, mas a conexão persiste
```

## Ver Também

- [AdsDisconnect]({{ site.baseurl }}/pt/funcoes/ads-disconnect/)
- [AdsCloseAllTables]({{ site.baseurl }}/pt/funcoes/ads-close-all-tables/)
- [AdsCloseCachedTables]({{ site.baseurl }}/pt/funcoes/ads-close-cached-tables/)

---

[AdsRestructureTable90 →]({{ site.baseurl }}/pt/funcoes/ads-restructure-table-90/)
