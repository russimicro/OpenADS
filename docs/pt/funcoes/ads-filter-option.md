---
title: AdsFilterOption
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-filter-option/
---

# AdsFilterOption

Retorna a opção de filtro.

## Sintaxe

```c
UNSIGNED32 AdsFilterOption(ADSHANDLE hTable, UNSIGNED16 usOption,
                           UNSIGNED16* pusValue);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `usOption` | `UNSIGNED16` | Opção (reservada). |
| `pusValue` | `UNSIGNED16*` | Ponteiro para receber o valor. |

## Valor de Retorno

`AE_SUCCESS` (0) sempre.

## Descrição

`AdsFilterOption` retorna a opção de filtro. No OpenADS, retorna sempre 0.

## Exemplo

```c
UNSIGNED16 usValue;
AdsFilterOption(hTable, 0, &usValue);
// usValue é 0
```

## Ver Também

- [AdsSetFilter]({{ site.baseurl }}/pt/funcoes/ads-set-filter/)
- [AdsGetFilter]({{ site.baseurl }}/pt/funcoes/ads-get-filter/)
- [AdsSetAOF]({{ site.baseurl }}/pt/funcoes/ads-set-aof/)

---

[AdsGetAOF →]({{ site.baseurl }}/pt/funcoes/ads-get-aof/)
