---
title: AdsClearScope
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-clear-scope/
---

# AdsClearScope

Remove o scope (topo ou fundo) de um índice.

## Sintaxe

```c
UNSIGNED32 AdsClearScope(ADSHANDLE hIndex, UNSIGNED16 usScope);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hIndex` | `ADSHANDLE` | Handle do índice. |
| `usScope` | `UNSIGNED16` | Tipo de scope: `ADS_TOP` para o topo, outro valor para o fundo. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsClearScope` remove o scope (topo ou fundo) definido para o índice, permitindo navegação completa.

## Exemplo

```c
AdsClearScope(hIndex, ADS_TOP);   // Remove scope superior
AdsClearScope(hIndex, ADS_BOTTOM); // Remove scope inferior
```

## Ver Também

- [AdsSetScope]({{ site.baseurl }}/pt/funcoes/ads-set-scope/)
- [AdsGetScope]({{ site.baseurl }}/pt/funcoes/ads-get-scope/)
- [AdsClearAllScopes]({{ site.baseurl }}/pt/funcoes/ads-clear-all-scopes/)

---

[AdsClearAllScopes →]({{ site.baseurl }}/pt/funcoes/ads-clear-all-scopes/)
