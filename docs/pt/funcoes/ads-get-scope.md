---
title: AdsGetScope
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-scope/
---

# AdsGetScope

Retorna o valor do scope (topo ou fundo).

## Sintaxe

```c
UNSIGNED32 AdsGetScope(ADSHANDLE hIndex, UNSIGNED16 usScope,
                       UNSIGNED8* pucBuf, UNSIGNED16* pusLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hIndex` | `ADSHANDLE` | Handle do índice. |
| `usScope` | `UNSIGNED16` | Tipo de scope: `ADS_TOP` para o topo, outro valor para o fundo. |
| `pucBuf` | `UNSIGNED8*` | Buffer para receber o valor do scope. |
| `pusLen` | `UNSIGNED16*` | Ponteiro para o tamanho do buffer. Na saída, contém o comprimento. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsGetScope` retorna o valor do scope (topo ou fundo) definido para o índice. O scope limita a navegação aos registos que satisfazem a condição.

## Exemplo

```c
UNSIGNED8 szScope[256];
UNSIGNED16 usLen = sizeof(szScope);
AdsGetScope(hIndex, ADS_TOP, szScope, &usLen);
// szScope contém o valor do scope superior
```

## Ver Também

- [AdsSetScope]({{ site.baseurl }}/pt/funcoes/ads-set-scope/)
- [AdsClearScope]({{ site.baseurl }}/pt/funcoes/ads-clear-scope/)
- [AdsClearAllScopes]({{ site.baseurl }}/pt/funcoes/ads-clear-all-scopes/)

---

[AdsSetScope →]({{ site.baseurl }}/pt/funcoes/ads-set-scope/)
