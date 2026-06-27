---
title: AdsSetScopedRelation
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-set-scoped-relation/
---

# AdsSetScopedRelation

Estabelece uma relação pai-filho que também limita o filho ao grupo de chave correspondente.

## Sintaxe

```c
UNSIGNED32 AdsSetScopedRelation(ADSHANDLE hTableParent, ADSHANDLE hTableChild, UNSIGNED8 *pucExpr);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTableParent` | `ADSHANDLE` | Handle da tabela pai (controladora). |
| `hTableChild` | `ADSHANDLE` | Handle da tabela filha (relacionada). |
| `pucExpr` | `UNSIGNED8*` | Expressão de relação, avaliada sobre o registo atual do pai para produzir a chave do filho. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_FUNCTION_NOT_AVAILABLE` (5004) para tabelas remotas. `AE_INTERNAL_ERROR` (5000) se a expressão for nula ou o handle for desconhecido.

## Descrição

`AdsSetScopedRelation` funciona tal como `AdsSetRelation` — ao mover o pai, o filho é reposicionado avaliando a expressão de relação sobre o registo atual do pai —, mas além disso **limita** o filho de modo que apenas os registos cuja chave corresponde ao valor do pai ficam visíveis.

Quando o pai se move, é fixado o âmbito (scope) superior e inferior da ordem controladora do filho igual à chave de relação, e o filho é posicionado no primeiro registo desse grupo. A navegação no filho (`AdsGotoTop`, `AdsGotoBottom`, `AdsSkip`) permanece então dentro do grupo correspondente, que é a forma natural de percorrer o lado "muitos" de uma relação um-para-muitos.

O filho deve ter uma ordem controladora para que o âmbito tenha efeito; sem ela, a relação degrada para um simples movimento por número de registo (igual a `AdsSetRelation`). `AdsClearRelation` remove a relação e liberta o âmbito imposto. Fechar qualquer uma das tabelas também liberta o âmbito. Não está disponível para tabelas remotas.

## Exemplo

```c
// Para cada FATURA, percorrer apenas as suas próprias LINHAS.
AdsSetScopedRelation(hFaturas, hLinhas, "INV_NO");

AdsGotoTop(hFaturas);
AdsGotoTop(hLinhas);
while (1) {
    UNSIGNED16 bEof;
    AdsAtEOF(hLinhas, &bEof);    // EOF no fim das linhas DESTA fatura
    if (bEof) break;
    // ... processar a linha ...
    AdsSkip(hLinhas, 1);
}
```

## Ver Também

- [AdsSetRelation]({{ site.baseurl }}/pt/funcoes/ads-set-relation/)
- [AdsClearRelation]({{ site.baseurl }}/pt/funcoes/ads-clear-relation/)
- [AdsSetScope]({{ site.baseurl }}/pt/funcoes/ads-set-scope/)

---

[← AdsSetRelation]({{ site.baseurl }}/pt/funcoes/ads-set-relation/)
