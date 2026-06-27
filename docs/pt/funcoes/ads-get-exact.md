---
title: AdsGetExact
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-exact/
---

# AdsGetExact

Retorna se a comparação de strings é exata.

## Sintaxe

```c
UNSIGNED32 AdsGetExact(UNSIGNED16* p);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `p` | `UNSIGNED16*` | Ponteiro para receber 1 se exato, 0 caso contrário. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsGetExact` retorna se as comparações de strings são exatas (SET EXACT ON/OFF).

## Exemplo

```c
UNSIGNED16 usExact;
AdsGetExact(&usExact);
// usExact é 1 (exato) ou 0 (parcial)
```

## Ver Também

- [AdsSetExact]({{ site.baseurl }}/pt/funcoes/ads-set-exact/)
- [AdsSeek]({{ site.baseurl }}/pt/funcoes/ads-seek/)
- [AdsSetFilter]({{ site.baseurl }}/pt/funcoes/ads-set-filter/)

---

[AdsGetDeleted →]({{ site.baseurl }}/pt/funcoes/ads-get-deleted/)
