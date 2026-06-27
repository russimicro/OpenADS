---
title: AdsEvalTestExpr
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-eval-test-expr/
---

# AdsEvalTestExpr

Testa se uma expressão é válida.

## Sintaxe

```c
UNSIGNED32 AdsEvalTestExpr(ADSHANDLE hTable, UNSIGNED8* pucExpr,
                           UNSIGNED16* pusType);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucExpr` | `UNSIGNED8*` | Expressão a testar. |
| `pusType` | `UNSIGNED16*` | Ponteiro para receber o tipo da expressão. |

## Valor de Retorno

`AE_SUCCESS` (0) sempre.

## Descrição

`AdsEvalTestExpr` testa se uma expressão é válida. No OpenADS, retorna sempre sucesso (0).

## Exemplo

```c
UNSIGNED16 usType;
AdsEvalTestExpr(hTable, "Idade > 18", &usType);
```

## Ver Também

- [AdsEvalLogicalExpr]({{ site.baseurl }}/pt/funcoes/ads-eval-logical-expr/)
- [AdsEvalNumericExpr]({{ site.baseurl }}/pt/funcoes/ads-eval-numeric-expr/)
- [AdsEvalStringExpr]({{ site.baseurl }}/pt/funcoes/ads-eval-string-expr/)

---

[AdsExecuteSQL →]({{ site.baseurl }}/pt/funcoes/ads-execute-sql/)
