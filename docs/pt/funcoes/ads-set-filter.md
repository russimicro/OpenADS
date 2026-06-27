---
title: AdsSetFilter
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-set-filter/
---

# AdsSetFilter

Define um filtro Clipper para a tabela.

## Sintaxe

```c
UNSIGNED32 AdsSetFilter(ADSHANDLE hTable, UNSIGNED8* pucFilter);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucFilter` | `UNSIGNED8*` | Expressão do filtro. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se a expressão for nula.

## Descrição

`AdsSetFilter` define um filtro Clipper que limita a navegação aos registos que satisfazem a expressão.

Para backends SQL, a expressão é traduzida para SQL WHERE quando possível.

Para tabelas remotas, a expressão é armazenada localmente.

## Exemplo

```c
AdsSetFilter(hTable, "Idade > 18");
// Apenas registos com Idade > 18 são visíveis
```

## Ver Também

- [AdsClearFilter]({{ site.baseurl }}/pt/funcoes/ads-clear-filter/)
- [AdsGetFilter]({{ site.baseurl }}/pt/funcoes/ads-get-filter/)
- [AdsSetAOF]({{ site.baseurl }}/pt/funcoes/ads-set-aof/)

---

[AdsClearFilter →]({{ site.baseurl }}/pt/funcoes/ads-clear-filter/)
