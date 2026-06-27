---
title: AdsSetExact
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-set-exact/
---

# AdsSetExact

Define se a comparação de strings é exata.

## Sintaxe

```c
UNSIGNED32 AdsSetExact(UNSIGNED16 us);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `us` | `UNSIGNED16` | 1 para comparação exata, 0 para parcial. |

## Valor de Retorno

`AE_SUCCESS` (0) sempre.

## Descrição

`AdsSetExact` define se as comparações de strings são exatas (SET EXACT ON/OFF).

## Exemplo

```c
AdsSetExact(1);  // Comparação exata
```

## Ver Também

- [AdsGetExact]({{ site.baseurl }}/pt/funcoes/ads-get-exact/)
- [AdsSetFilter]({{ site.baseurl }}/pt/funcoes/ads-set-filter/)
- [AdsSeek]({{ site.baseurl }}/pt/funcoes/ads-seek/)

---

[AdsGetExact →]({{ site.baseurl }}/pt/funcoes/ads-get-exact/)
