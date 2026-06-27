---
title: AdsGetBookmark
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-bookmark/
---

# AdsGetBookmark

Retorna um bookmark para a posição atual.

## Sintaxe

```c
UNSIGNED32 AdsGetBookmark(ADSHANDLE hTable, ADSHANDLE* phBookmark);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `phBookmark` | `ADSHANDLE*` | Ponteiro para receber o bookmark. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsGetBookmark` retorna um bookmark que representa a posição atual do cursor. O bookmark pode ser usado com `AdsGotoBookmark60` para voltar à mesma posição.

O bookmark implementado é o número do registo (recno-as-token).

## Exemplo

```c
ADSHANDLE hBookmark;
AdsGetBookmark(hTable, &hBookmark);
// Navegar...
AdsGotoBookmark60(hTable, hBookmark);
// Voltar à posição original
```

## Ver Também

- [AdsGotoBookmark60]({{ site.baseurl }}/pt/funcoes/ads-goto-bookmark-60/)
- [AdsGetRecordNum]({{ site.baseurl }}/pt/funcoes/ads-get-record-num/)
- [AdsGotoRecord]({{ site.baseurl }}/pt/funcoes/ads-goto-record/)

---

[AdsGotoBookmark60 →]({{ site.baseurl }}/pt/funcoes/ads-goto-bookmark-60/)
