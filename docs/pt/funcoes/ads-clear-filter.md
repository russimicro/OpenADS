---
title: AdsClearFilter
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-clear-filter/
---

# AdsClearFilter

Remove o filtro ativo da tabela.

## Sintaxe

```c
UNSIGNED32 AdsClearFilter(ADSHANDLE hTable);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |

## Valor de Retorno

`AE_SUCCESS` (0) sempre.

## Descrição

`AdsClearFilter` remove o filtro Clipper ativo da tabela, tornando todos os registos visíveis novamente.

**Nota:** No OpenADS, esta função é uma operação de no-op (retorna sucesso sem alterar nada).

## Exemplo

```c
AdsClearFilter(hTable);
// Todos os registos são visíveis novamente
```

## Ver Também

- [AdsSetFilter]({{ site.baseurl }}/pt/funcoes/ads-set-filter/)
- [AdsGetFilter]({{ site.baseurl }}/pt/funcoes/ads-get-filter/)
- [AdsClearAOF]({{ site.baseurl }}/pt/funcoes/ads-clear-aof/)

---

[AdsGetFilter →]({{ site.baseurl }}/pt/funcoes/ads-get-filter/)
