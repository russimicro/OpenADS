---
title: AdsClearAllScopes
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-clear-all-scopes/
---

# AdsClearAllScopes

Remove todos os scopes de uma tabela.

## Sintaxe

```c
UNSIGNED32 AdsClearAllScopes(ADSHANDLE hTable);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |

## Valor de Retorno

`AE_SUCCESS` (0) sempre.

## Descrição

`AdsClearAllScopes` remove todos os scopes (topo e fundo) de todos os índices associados à tabela, permitindo navegação completa.

**Nota:** No OpenADS, esta função é uma operação de no-op (retorna sucesso sem alterar nada), pois os scopes são geridos individualmente por índice.

## Exemplo

```c
AdsClearAllScopes(hTable);
```

## Ver Também

- [AdsSetScope]({{ site.baseurl }}/pt/funcoes/ads-set-scope/)
- [AdsClearScope]({{ site.baseurl }}/pt/funcoes/ads-clear-scope/)
- [AdsGetScope]({{ site.baseurl }}/pt/funcoes/ads-get-scope/)

---

[AdsSeek →]({{ site.baseurl }}/pt/funcoes/ads-seek/)
