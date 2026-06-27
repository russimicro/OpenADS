---
title: AdsEvalAOF
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-eval-aof/
---

# AdsEvalAOF

Avalia uma expressão AOF.

## Sintaxe

```c
UNSIGNED32 AdsEvalAOF(ADSHANDLE hTable, UNSIGNED8* pucExpr,
                      UNSIGNED16* pusOptLevel);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucExpr` | `UNSIGNED8*` | Expressão AOF. |
| `pusOptLevel` | `UNSIGNED16*` | Ponteiro para receber o nível de otimização. |

## Valor de Retorno

`AE_SUCCESS` (0) sempre.

## Descrição

`AdsEvalAOF` avalia uma expressão AOF e retorna o nível de otimização. No OpenADS, retorna 0 (ADS_OPTIMIZED_NONE).

## Exemplo

```c
UNSIGNED16 usLevel;
AdsEvalAOF(hTable, "Idade > 18", &usLevel);
// usLevel é 0 (sem otimização)
```

## Ver Também

- [AdsSetAOF]({{ site.baseurl }}/pt/funcoes/ads-set-aof/)
- [AdsGetAOF]({{ site.baseurl }}/pt/funcoes/ads-get-aof/)
- [AdsGetAOFOptLevel]({{ site.baseurl }}/pt/funcoes/ads-get-aof-opt-level/)

---

[AdsFilterOption →]({{ site.baseurl }}/pt/funcoes/ads-filter-option/)
