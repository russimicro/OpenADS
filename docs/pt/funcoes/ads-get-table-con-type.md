---
title: AdsGetTableConType
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-table-con-type/
---

# AdsGetTableConType

Retorna o tipo de conexão da tabela.

## Sintaxe

```c
UNSIGNED32 AdsGetTableConType(ADSHANDLE hTable, UNSIGNED16* p);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `p` | `UNSIGNED16*` | Ponteiro para receber o tipo. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsGetTableConType` retorna o tipo de conexão da tabela. Delega para `AdsGetTableType`.

## Exemplo

```c
UNSIGNED16 usConType;
AdsGetTableConType(hTable, &usConType);
```

## Ver Também

- [AdsGetTableType]({{ site.baseurl }}/pt/funcoes/ads-get-table-type/)
- [AdsGetTableConnection]({{ site.baseurl }}/pt/funcoes/ads-get-table-connection/)
- [AdsGetConnectionType]({{ site.baseurl }}/pt/funcoes/ads-get-connection-type/)

---

[AdsGetTableConnection →]({{ site.baseurl }}/pt/funcoes/ads-get-table-connection/)
