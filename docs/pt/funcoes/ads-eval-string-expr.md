---
title: AdsEvalStringExpr
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-eval-string-expr/
---

# AdsEvalStringExpr

Avalia uma expressão de string.

## Sintaxe

```c
UNSIGNED32 AdsEvalStringExpr(ADSHANDLE hTable, UNSIGNED8* pucExpr,
                             UNSIGNED8* pucResult, UNSIGNED16* pusLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucExpr` | `UNSIGNED8*` | Expressão de string ou nome do campo. |
| `pucResult` | `UNSIGNED8*` | Buffer para receber o resultado. |
| `pusLen` | `UNSIGNED16*` | Ponteiro para o tamanho do buffer. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se a expressão for nula.

## Descrição

`AdsEvalStringExpr` avalia uma expressão de string ou retorna o valor de um campo.

## Exemplo

```c
UNSIGNED8 szResult[256];
UNSIGNED16 usLen = sizeof(szResult);
AdsEvalStringExpr(hTable, "Nome", szResult, &usLen);
// szResult contém o valor do campo Nome
```

## Ver Também

- [AdsEvalLogicalExpr]({{ site.baseurl }}/pt/funcoes/ads-eval-logical-expr/)
- [AdsEvalNumericExpr]({{ site.baseurl }}/pt/funcoes/ads-eval-numeric-expr/)
- [AdsEvalTestExpr]({{ site.baseurl }}/pt/funcoes/ads-eval-test-expr/)

---

[AdsEvalTestExpr →]({{ site.baseurl }}/pt/funcoes/ads-eval-test-expr/)
