---
title: AdsClearRelation
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-clear-relation/
---

# AdsClearRelation

Remove todas as relações estabelecidas a partir de uma tabela pai.

## Sintaxe

```c
UNSIGNED32 AdsClearRelation(ADSHANDLE hTableParent);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTableParent` | `ADSHANDLE` | Handle da tabela pai cujas relações são removidas. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso.

## Descrição

`AdsClearRelation` remove todas as relações pai-filho estabelecidas previamente a partir de `hTableParent` com `AdsSetRelation`. Após a chamada, mover o cursor do pai já não reposiciona os filhos antes relacionados; cada filho mantém a sua posição atual.

Apenas são removidas as relações que esta tabela controla **como pai**. Se a mesma tabela for também filha de outra área de trabalho, esse vínculo permanece intacto. As relações também são descartadas automaticamente ao fechar a tabela pai ou qualquer filha.

## Exemplo

```c
AdsSetRelation(hOrders, hCustomers, "CUST_ID");
// ... navegar com a relação ativa ...

AdsClearRelation(hOrders);    // o filho já não segue o pai
```

## Ver Também

- [AdsSetRelation]({{ site.baseurl }}/pt/funcoes/ads-set-relation/)
- [AdsClearScope]({{ site.baseurl }}/pt/funcoes/ads-clear-scope/)

---

[← AdsSetRelation]({{ site.baseurl }}/pt/funcoes/ads-set-relation/)
