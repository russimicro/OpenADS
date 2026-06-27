---
title: AdsAggregate
layout: default
parent: Referência da API
nav_order: 2
permalink: /pt/funcoes/ads-aggregate/
---

# AdsAggregate

Executa uma agregação no servidor.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsAggregate(ADSHANDLE  hTbl,
                                   UNSIGNED8* pszForCond,
                                   UNSIGNED8* pszAggSpec,
                                   ADSHANDLE* phResult);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTbl` | `ADSHANDLE` | Handle da tabela. |
| `pszForCond` | `UNSIGNED8*` | Condição FOR no estilo xBase (vazio = todas as linhas). |
| `pszAggSpec` | `UNSIGNED8*` | Lista separada por `;` de itens no formato `FN:CAMPO` (ex: `COUNT:;SUM:QTY;MIN:NM`). |
| `phResult` | `ADSHANDLE*` | Ponteiro para receber o handle do resultado. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_FUNCTION_NOT_AVAILABLE` (5004) para tabelas locais.

## Descrição

`AdsAggregate` executa uma agregação no servidor. Para tabelas remotas, o servidor processa a agregação. Para tabelas locais, retorna `AE_FUNCTION_NOT_AVAILABLE` e o chamador deve usar um loop de totalização do lado do cliente.

Suporta: COUNT, SUM, AVG, MIN, MAX.

## Exemplo

```c
ADSHANDLE hResult;
AdsAggregate(hTbl, "", "COUNT:;SUM:QTY", &hResult);
```

## Ver Também

- [AdsAggregateCount]({{ site.baseurl }}/pt/funcoes/ads-aggregate-count/)
- [AdsAggregateValue]({{ site.baseurl }}/pt/funcoes/ads-aggregate-value/)
- [AdsAggregateClose]({{ site.baseurl }}/pt/funcoes/ads-aggregate-close/)

---

[AdsAggregateClose →]({{ site.baseurl }}/pt/funcoes/ads-aggregate-close/)
