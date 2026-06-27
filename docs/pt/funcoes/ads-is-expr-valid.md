---
title: AdsIsExprValid
layout: default
parent: Referência da API
nav_order: 11
permalink: /pt/funcoes/ads-is-expr-valid/
---

# AdsIsExprValid

Verifica se uma expressão é válida para uma tabela.

## Sintaxe

```c
UNSIGNED32 AdsIsExprValid(ADSHANDLE hTable, UNSIGNED8* pucExpr,
                          UNSIGNED16* pbValid);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucExpr` | `UNSIGNED8*` | Expressão a ser validada. |
| `pbValid` | `UNSIGNED16*` | Ponteiro para variável que recebe o resultado (1=válida, 0=inválida). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsIsExprValid` verifica se a expressão especificada pode ser avaliada no contexto da tabela atual. Útil para validar expressões antes de usá-las em filtros, índices ou consultas.

## Exemplo

```c
UNSIGNED16 bValid;
AdsIsExprValid(hTable, "Preco > 100", &bValid);
if (bValid) {
    // Expressão é válida, pode ser usada em filtros
}
```

## Ver Também

- [AdsSetFilter]({{ site.baseurl }}/pt/funcoes/ads-set-filter/)
- [AdsSetAOF]({{ site.baseurl }}/pt/funcoes/ads-set-aof/)
- [AdsEvalTestExpr]({{ site.baseurl }}/pt/funcoes/ads-eval-test-expr/)

---

[AdsMgConnect →]({{ site.baseurl }}/pt/funcoes/ads-mg-connect/)
