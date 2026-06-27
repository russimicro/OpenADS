---
title: AdsGetTableOpenOptions
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-table-open-options/
---

# AdsGetTableOpenOptions

Retorna as opções de abertura da tabela.

## Sintaxe

```c
UNSIGNED32 AdsGetTableOpenOptions(ADSHANDLE hTable, UNSIGNED32* pulOptions);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pulOptions` | `UNSIGNED32*` | Ponteiro para receber as opções. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsGetTableOpenOptions` retorna as opções de abertura da tabela:
- `ADS_READONLY` (1) - Apenas leitura
- `ADS_SHARED` (2) - Partilhada
- `ADS_EXCLUSIVE` (4) - Exclusiva

## Exemplo

```c
UNSIGNED32 ulOptions;
AdsGetTableOpenOptions(hTable, &ulOptions);
// ulOptions contém as opções de abertura
```

## Ver Também

- [AdsOpenTable]({{ site.baseurl }}/pt/funcoes/ads-open-table/)
- [AdsGetTableType]({{ site.baseurl }}/pt/funcoes/ads-get-table-type/)
- [AdsGetTableAlias]({{ site.baseurl }}/pt/funcoes/ads-get-table-alias/)

---

[AdsGetTableHandle25 →]({{ site.baseurl }}/pt/funcoes/ads-get-table-handle-25/)
