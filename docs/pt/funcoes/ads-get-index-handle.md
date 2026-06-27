---
title: AdsGetIndexHandle
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-index-handle/
---

# AdsGetIndexHandle

Retorna o handle de um índice pelo seu nome.

## Sintaxe

```c
UNSIGNED32 AdsGetIndexHandle(ADSHANDLE hTable, UNSIGNED8* pucName,
                             ADSHANDLE* phIndex);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucName` | `UNSIGNED8*` | Nome do índice (tag). |
| `phIndex` | `ADSHANDLE*` | Ponteiro para receber o handle do índice. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o índice não for encontrado.

## Descrição

`AdsGetIndexHandle` retorna o handle de um índice previamente aberto pelo seu nome (tag). O nome pode conter espaços à direita que serão removidos.

## Exemplo

```c
ADSHANDLE hIndex;
AdsGetIndexHandle(hTable, "Nome", &hIndex);
// hIndex contém o handle do índice
```

## Ver Também

- [AdsOpenIndex]({{ site.baseurl }}/pt/funcoes/ads-open-index/)
- [AdsCloseIndex]({{ site.baseurl }}/pt/funcoes/ads-close-index/)
- [AdsGetIndexName]({{ site.baseurl }}/pt/funcoes/ads-get-index-name/)

---

[AdsGetIndexName →]({{ site.baseurl }}/pt/funcoes/ads-get-index-name/)
