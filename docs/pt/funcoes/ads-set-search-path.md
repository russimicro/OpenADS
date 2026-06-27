---
title: AdsSetSearchPath
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-set-search-path/
---

# AdsSetSearchPath

Define o caminho de procura.

## Sintaxe

```c
UNSIGNED32 AdsSetSearchPath(UNSIGNED8* pucPath);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `pucPath` | `UNSIGNED8*` | Caminho de procura. |

## Valor de Retorno

`AE_SUCCESS` (0) sempre.

## Descrição

`AdsSetSearchPath` define o caminho de procura para localização de tabelas.

## Exemplo

```c
AdsSetSearchPath("/caminho/dados;/outro/caminho");
```

## Ver Também

- [AdsGetSearchPath]({{ site.baseurl }}/pt/funcoes/ads-get-search-path/)
- [AdsSetDefault]({{ site.baseurl }}/pt/funcoes/ads-set-default/)
- [AdsGetDefault]({{ site.baseurl }}/pt/funcoes/ads-get-default/)

---

[AdsSetServerType →]({{ site.baseurl }}/pt/funcoes/ads-set-server-type/)
