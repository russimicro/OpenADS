---
title: AdsSetRelation
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-set-relation/
---

# AdsSetRelation

Estabelece uma relação pai-filho entre áreas de trabalho para que o filho siga o pai.

## Sintaxe

```c
UNSIGNED32 AdsSetRelation(ADSHANDLE hTableParent, ADSHANDLE hTableChild, UNSIGNED8 *pucExpr);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTableParent` | `ADSHANDLE` | Handle da tabela pai (controladora). |
| `hTableChild` | `ADSHANDLE` | Handle da tabela filha (relacionada). |
| `pucExpr` | `UNSIGNED8*` | Expressão de relação, avaliada sobre o registo atual do pai para produzir a chave de procura do filho. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_FUNCTION_NOT_AVAILABLE` (5004) para tabelas remotas. `AE_INTERNAL_ERROR` (5000) se a expressão for nula ou o handle for desconhecido.

## Descrição

`AdsSetRelation` liga uma tabela filha a uma pai, de modo que sempre que o cursor do pai se move, o filho é reposicionado automaticamente. Após cada navegação do pai (`AdsGotoTop`, `AdsGotoBottom`, `AdsSkip`, `AdsGotoRecord`, `AdsSeek`), a expressão de relação é avaliada sobre o registo atual do pai e o resultado é usado para procurar no filho:

- Se o filho tiver uma ordem controladora, o valor é procurado nesse índice (procura suave). Uma falha deixa o filho em EOF.
- Se o filho não tiver ordem controladora, o valor é tratado como um número de registo e o filho é movido para ele.

O filho é posicionado de imediato ao estabelecer a relação, usando o registo atual do pai. Vários filhos podem ser relacionados com o mesmo pai, e as relações propagam-se em cadeias pai-filho. Um pai pode estabelecer mais do que uma relação; use `AdsClearRelation` para as remover. Fechar qualquer uma das tabelas descarta as relações afetadas. Não está disponível para tabelas remotas.

## Exemplo

```c
// Pai ORDERS, filho CUSTOMERS indexado pelo seu ID numérico.
AdsSetRelation(hOrders, hCustomers, "CUST_ID");

AdsGotoTop(hOrders);          // o filho procura o cliente correspondente
// ... ler campos de hCustomers para o pedido atual ...
AdsSkip(hOrders, 1);          // o filho segue automaticamente
```

## Ver Também

- [AdsClearRelation]({{ site.baseurl }}/pt/funcoes/ads-clear-relation/)
- [AdsSeek]({{ site.baseurl }}/pt/funcoes/ads-seek/)
- [AdsSetIndexOrder]({{ site.baseurl }}/pt/funcoes/ads-set-index-order/)

---

[AdsClearRelation →]({{ site.baseurl }}/pt/funcoes/ads-clear-relation/)
