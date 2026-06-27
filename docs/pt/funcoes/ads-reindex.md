---
title: AdsReindex
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-reindex/
---

# AdsReindex

Reconstrói todos os índices da tabela.

## Sintaxe

```c
UNSIGNED32 AdsReindex(ADSHANDLE hTable);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsReindex` reconstrói todos os índices associados à tabela. Útil após alterações significativas aos dados ou quando os índices estão corrompidos.

Para tabelas remotas, a operação é executada no servidor.

## Exemplo

```c
AdsReindex(hTable);
```

## Ver Também

- [AdsCreateIndex]({{ site.baseurl }}/pt/funcoes/ads-create-index/)
- [AdsOpenIndex]({{ site.baseurl }}/pt/funcoes/ads-open-index/)
- [AdsCloseIndex]({{ site.baseurl }}/pt/funcoes/ads-close-index/)

---

[AdsSetFilter →]({{ site.baseurl }}/pt/funcoes/ads-set-filter/)
