---
title: AdsGetExact22
layout: default
parent: Referência da API
nav_order: 3
permalink: /pt/funcoes/ads-get-exact-22/
---

# AdsGetExact22

Retorna se a busca exata (EXACT) está habilitada para um handle.

## Sintaxe

```c
UNSIGNED32 AdsGetExact22(ADSHANDLE hObj, UNSIGNED16* pbExact);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hObj` | `ADSHANDLE` | Handle de tabela, índice ou conexão. |
| `pbExact` | `UNSIGNED16*` | Ponteiro para variável que recebe o estado (1=exato, 0=parcial). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsGetExact22` verifica se o modo de busca exata está habilitado. Quando habilitado, as buscas por strings requerem correspondência exata, sem correspondência parcial por prefixo.

## Exemplo

```c
UNSIGNED16 bExact;
AdsGetExact22(hTable, &bExact);
if (bExact) {
    // Busca exata está habilitada
}
```

## Ver Também

- [AdsSetExact]({{ site.baseurl }}/pt/funcoes/ads-set-exact/)
- [AdsGetExact]({{ site.baseurl }}/pt/funcoes/ads-get-exact/)
- [AdsSeek]({{ site.baseurl }}/pt/funcoes/ads-seek/)

---

[AdsGetFieldW →]({{ site.baseurl }}/pt/funcoes/ads-get-field-w/)
