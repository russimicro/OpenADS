---
title: AdsEvalLogicalExpr
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-eval-logical-expr/
---

# AdsEvalLogicalExpr

Avalia uma expressão lógica.

## Sintaxe

```c
UNSIGNED32 AdsEvalLogicalExpr(ADSHANDLE hTable, UNSIGNED8* pucExpr,
                              UNSIGNED16* pbResult);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucExpr` | `UNSIGNED8*` | Expressão lógica. |
| `pbResult` | `UNSIGNED16*` | Ponteiro para receber o resultado (1=verdadeiro, 0=falso). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se a expressão for nula.

## Descrição

`AdsEvalLogicalExpr` avalia uma expressão lógica contra o registo atual.

## Exemplo

```c
UNSIGNED16 pbResult;
AdsEvalLogicalExpr(hTable, "Idade > 18", &pbResult);
// pbResult é 1 se verdadeiro, 0 se falso
```

## Ver Também

- [AdsEvalNumericExpr]({{ site.baseurl }}/pt/funcoes/ads-eval-numeric-expr/)
- [AdsEvalStringExpr]({{ site.baseurl }}/pt/funcoes/ads-eval-string-expr/)
- [AdsEvalTestExpr]({{ site.baseurl }}/pt/funcoes/ads-eval-test-expr/)

---

[AdsEvalNumericExpr →]({{ site.baseurl }}/pt/funcoes/ads-eval-numeric-expr/)
