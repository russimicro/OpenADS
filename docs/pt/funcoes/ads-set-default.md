---
title: AdsSetDefault
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-set-default/
---

# AdsSetDefault

Define o caminho padrão.

## Sintaxe

```c
UNSIGNED32 AdsSetDefault(UNSIGNED8* pucPath);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `pucPath` | `UNSIGNED8*` | Caminho padrão. |

## Valor de Retorno

`AE_SUCCESS` (0) sempre.

## Descrição

`AdsSetDefault` define o caminho padrão para localização de tabelas.

## Exemplo

```c
AdsSetDefault("/caminho/dados");
```

## Ver Também

- [AdsGetDefault]({{ site.baseurl }}/pt/funcoes/ads-get-default/)
- [AdsSetSearchPath]({{ site.baseurl }}/pt/funcoes/ads-set-search-path/)
- [AdsGetSearchPath]({{ site.baseurl }}/pt/funcoes/ads-get-search-path/)

---

[AdsSetEpoch →]({{ site.baseurl }}/pt/funcoes/ads-set-epoch/)
