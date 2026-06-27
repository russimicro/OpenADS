---
title: AdsClearAOF
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-clear-aof/
---

# AdsClearAOF

Remove a AOF ativa.

## Sintaxe

```c
UNSIGNED32 AdsClearAOF(ADSHANDLE hTable);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsClearAOF` remove a AOF ativa, tornando todos os registos visíveis novamente.

## Exemplo

```c
AdsClearAOF(hTable);
// Todos os registos são visíveis novamente
```

## Ver Também

- [AdsSetAOF]({{ site.baseurl }}/pt/funcoes/ads-set-aof/)
- [AdsGetAOF]({{ site.baseurl }}/pt/funcoes/ads-get-aof/)
- [AdsRefreshAOF]({{ site.baseurl }}/pt/funcoes/ads-refresh-aof/)

---

[AdsGetAOF →]({{ site.baseurl }}/pt/funcoes/ads-get-aof/)
