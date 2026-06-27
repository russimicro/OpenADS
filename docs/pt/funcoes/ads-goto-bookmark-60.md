---
title: AdsGotoBookmark60
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-goto-bookmark-60/
---

# AdsGotoBookmark60

Posiciona no registo indicado pelo bookmark (versão 6.0+).

## Sintaxe

```c
UNSIGNED32 AdsGotoBookmark60(ADSHANDLE hObj, UNSIGNED8* pucBookmark,
                             UNSIGNED32 ulLength);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hObj` | `ADSHANDLE` | Handle da tabela. |
| `pucBookmark` | `UNSIGNED8*` | Buffer com o bookmark (4 bytes). |
| `ulLength` | `UNSIGNED32` | Comprimento do bookmark (mínimo 4). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o bookmark for inválido.

## Descrição

`AdsGotoBookmark60` move o cursor para o registo indicado pelo bookmark. O bookmark é o número do registo em formato binário little-endian.

## Exemplo

```c
UNSIGNED8 bookmark[4];
AdsGetBookmark60(hTable, bookmark, &ulLen);
// Navegar...
AdsGotoBookmark60(hTable, bookmark, 4);
```

## Ver Também

- [AdsGetBookmark60]({{ site.baseurl }}/pt/funcoes/ads-get-bookmark-60/)
- [AdsGotoRecord]({{ site.baseurl }}/pt/funcoes/ads-goto-record/)
- [AdsGetRecordNum]({{ site.baseurl }}/pt/funcoes/ads-get-record-num/)

---

[AdsGetMemoBlockSize →]({{ site.baseurl }}/pt/funcoes/ads-get-memo-block-size/)
