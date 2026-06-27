---
title: AdsGetSearchPath
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-search-path/
---

# AdsGetSearchPath

Retorna o caminho de procura.

## Sintaxe

```c
UNSIGNED32 AdsGetSearchPath(UNSIGNED8* p, UNSIGNED16* l);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `p` | `UNSIGNED8*` | Buffer para receber o caminho. |
| `l` | `UNSIGNED16*` | Ponteiro para o tamanho do buffer. Na saída, contém o comprimento. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsGetSearchPath` retorna o caminho de procura para localização de tabelas.

## Exemplo

```c
UNSIGNED8 szPath[256];
UNSIGNED16 usLen = sizeof(szPath);
AdsGetSearchPath(szPath, &usLen);
// szPath contém o caminho de procura
```

## Ver Também

- [AdsSetSearchPath]({{ site.baseurl }}/pt/funcoes/ads-set-search-path/)
- [AdsGetDefault]({{ site.baseurl }}/pt/funcoes/ads-get-default/)
- [AdsSetDefault]({{ site.baseurl }}/pt/funcoes/ads-set-default/)

---

[AdsSetSearchPath →]({{ site.baseurl }}/pt/funcoes/ads-set-search-path/)
