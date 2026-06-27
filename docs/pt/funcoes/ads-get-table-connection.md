---
title: AdsGetTableConnection
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-table-connection/
---

# AdsGetTableConnection

Retorna o handle da conexão da tabela.

## Sintaxe

```c
UNSIGNED32 AdsGetTableConnection(ADSHANDLE hTable, ADSHANDLE* p);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `p` | `ADSHANDLE*` | Ponteiro para receber o handle da conexão. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsGetTableConnection` retorna o handle da conexão associada à tabela.

## Exemplo

```c
ADSHANDLE hConn;
AdsGetTableConnection(hTable, &hConn);
// hConn contém o handle da conexão
```

## Ver Também

- [AdsGetConnectionType]({{ site.baseurl }}/pt/funcoes/ads-get-connection-type/)
- [AdsGetTableType]({{ site.baseurl }}/pt/funcoes/ads-get-table-type/)
- [AdsOpenTable]({{ site.baseurl }}/pt/funcoes/ads-open-table/)

---

[AdsGetTableOpenOptions →]({{ site.baseurl }}/pt/funcoes/ads-get-table-open-options/)
