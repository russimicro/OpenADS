---
title: AdsData
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-data/
---

# AdsData

Função de dados (reservada).

## Sintaxe

```c
UNSIGNED32 AdsData(UNSIGNED16 usOption, void* pData);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `usOption` | `UNSIGNED16` | Opção (reservada). |
| `pData` | `void*` | Dados (reservados). |

## Valor de Retorno

`AE_SUCCESS` (0) sempre.

## Descrição

`AdsData` é uma função reservada. No OpenADS, é uma operação de no-op.

## Ver Também

- [AdsEvalAOF]({{ site.baseurl }}/pt/funcoes/ads-eval-aof/)
- [AdsFilterOption]({{ site.baseurl }}/pt/funcoes/ads-filter-option/)

---

[AdsEvalAOF →]({{ site.baseurl }}/pt/funcoes/ads-eval-aof/)
