---
title: AdsSetProperty90
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-set-property-90/
---

# AdsSetProperty90

Define uma propriedade (versão 9.0+).

## Sintaxe

```c
UNSIGNED32 AdsSetProperty90(ADSHANDLE hObj, UNSIGNED32 ulOperation,
                            UNSIGNED64* puqValue);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hObj` | `ADSHANDLE` | Handle do objeto. |
| `ulOperation` | `UNSIGNED32` | Operação (reservada). |
| `puqValue` | `UNSIGNED64*` | Valor (reservado). |

## Valor de Retorno

`AE_SUCCESS` (0) sempre.

## Descrição

`AdsSetProperty90` define uma propriedade. No OpenADS, é uma operação de no-op.

## Ver Também

- [AdsGetVersion]({{ site.baseurl }}/pt/funcoes/ads-get-version/)
- [AdsGetTableOpenOptions]({{ site.baseurl }}/pt/funcoes/ads-get-table-open-options/)

---

[AdsFindConnection25 →]({{ site.baseurl }}/pt/funcoes/ads-find-connection-25/)
