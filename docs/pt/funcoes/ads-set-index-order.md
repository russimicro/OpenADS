---
title: AdsSetIndexOrder
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-set-index-order/
---

# AdsSetIndexOrder

Define a ordem ativa da tabela.

## Sintaxe

```c
UNSIGNED32 AdsSetIndexOrder(ADSHANDLE hTable, UNSIGNED8* pucName);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucName` | `UNSIGNED8*` | Nome do tag do índice (vazio para ordem natural). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsSetIndexOrder` define qual índice (tag) está ativo para a tabela. Se o nome for vazio, a tabela volta à ordem natural (por número de registo).

## Exemplo

```c
AdsSetIndexOrder(hTable, "Nome");  // Ordenar por Nome
AdsSetIndexOrder(hTable, "");      // Volta à ordem natural
```

## Ver Também

- [AdsGetIndexHandle]({{ site.baseurl }}/pt/funcoes/ads-get-index-handle/)
- [AdsGetIndexOrderByHandle]({{ site.baseurl }}/pt/funcoes/ads-get-index-order-by-handle/)
- [AdsSetIndexDirection]({{ site.baseurl }}/pt/funcoes/ads-set-index-direction/)

---

[AdsSetIndexDirection →]({{ site.baseurl }}/pt/funcoes/ads-set-index-direction/)
