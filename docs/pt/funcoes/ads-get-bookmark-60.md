---
title: AdsGetBookmark60
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-bookmark-60/
---

# AdsGetBookmark60

Retorna um bookmark para a posição atual (versão 6.0+).

## Sintaxe

```c
UNSIGNED32 AdsGetBookmark60(ADSHANDLE hObj, UNSIGNED8* pucBookmark,
                            UNSIGNED32* pulLength);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hObj` | `ADSHANDLE` | Handle da tabela. |
| `pucBookmark` | `UNSIGNED8*` | Buffer para receber o bookmark (4 bytes). |
| `pulLength` | `UNSIGNED32*` | Ponteiro para o tamanho do buffer. Na saída, contém 4. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o buffer for muito pequeno.

## Descrição

`AdsGetBookmark60` retorna um bookmark de 4 bytes (recno em little-endian) que pode ser usado com `AdsGotoBookmark60`.

## Exemplo

```c
UNSIGNED8 bookmark[4];
UNSIGNED32 ulLen = 4;
AdsGetBookmark60(hTable, bookmark, &ulLen);
// Navegar...
AdsGotoBookmark60(hTable, bookmark, 4);
```

## Ver Também

- [AdsGotoBookmark60]({{ site.baseurl }}/pt/funcoes/ads-goto-bookmark-60/)
- [AdsGetBookmark]({{ site.baseurl }}/pt/funcoes/ads-get-bookmark/)
- [AdsGetRecordNum]({{ site.baseurl }}/pt/funcoes/ads-get-record-num/)

---

[AdsGotoBookmark60 →]({{ site.baseurl }}/pt/funcoes/ads-goto-bookmark-60/)
