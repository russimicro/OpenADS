---
title: AdsSetScope
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-set-scope/
---

# AdsSetScope

Define o scope (topo ou fundo) para um índice.

## Sintaxe

```c
UNSIGNED32 AdsSetScope(ADSHANDLE hIndex, UNSIGNED16 usScope,
                       UNSIGNED8* pucScope, UNSIGNED16 usLen,
                       UNSIGNED16 usDataType);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hIndex` | `ADSHANDLE` | Handle do índice. |
| `usScope` | `UNSIGNED16` | Tipo de scope: `ADS_TOP` para o topo, outro valor para o fundo. |
| `pucScope` | `UNSIGNED8*` | Buffer com o valor do scope. |
| `usLen` | `UNSIGNED16` | Comprimento do valor. |
| `usDataType` | `UNSIGNED16` | Tipo de dados: `ADS_STRINGKEY` para texto, `ADS_DOUBLEKEY` para numérico. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsSetScope` define o scope (topo ou fundo) para um índice. O scope limita a navegação aos registos que satisfazem a condição.

Para índices numéricos CDX, a função converte o valor double para a codificação interna do FoxNumeric.

## Exemplo

```c
AdsSetScope(hIndex, ADS_TOP, "A", 1, ADS_STRINGKEY);
AdsSetScope(hIndex, ADS_BOTTOM, "Z", 1, ADS_STRINGKEY);
// Navegação limitada a registos entre "A" e "Z"
```

## Ver Também

- [AdsGetScope]({{ site.baseurl }}/pt/funcoes/ads-get-scope/)
- [AdsClearScope]({{ site.baseurl }}/pt/funcoes/ads-clear-scope/)
- [AdsClearAllScopes]({{ site.baseurl }}/pt/funcoes/ads-clear-all-scopes/)

---

[AdsClearScope →]({{ site.baseurl }}/pt/funcoes/ads-clear-scope/)
