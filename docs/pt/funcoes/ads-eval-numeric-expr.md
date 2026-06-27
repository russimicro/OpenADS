---
title: AdsEvalNumericExpr
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-eval-numeric-expr/
---

# AdsEvalNumericExpr

Avalia uma expressão numérica.

## Sintaxe

```c
UNSIGNED32 AdsEvalNumericExpr(ADSHANDLE hTable, UNSIGNED8* pucExpr,
                              double* pdResult);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucExpr` | `UNSIGNED8*` | Expressão numérica ou nome do campo. |
| `pdResult` | `double*` | Ponteiro para receber o resultado. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se a expressão for nula.

## Descrição

`AdsEvalNumericExpr` avalia uma expressão numérica ou retorna o valor de um campo.

## Exemplo

```c
double dResult;
AdsEvalNumericExpr(hTable, "Preco", &dResult);
// dResult contém o valor do campo Preco
```

## Ver Também

- [AdsEvalLogicalExpr]({{ site.baseurl }}/pt/funcoes/ads-eval-logical-expr/)
- [AdsEvalStringExpr]({{ site.baseurl }}/pt/funcoes/ads-eval-string-expr/)
- [AdsEvalTestExpr]({{ site.baseurl }}/pt/funcoes/ads-eval-test-expr/)

---

[AdsEvalStringExpr →]({{ site.baseurl }}/pt/funcoes/ads-eval-string-expr/)
