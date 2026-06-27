---
title: AdsGetMemoBlockSize
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-memo-block-size/
---

# AdsGetMemoBlockSize

Retorna o tamanho do bloco memo.

## Sintaxe

```c
UNSIGNED32 AdsGetMemoBlockSize(ADSHANDLE hObj, UNSIGNED16* pusBlockSize);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hObj` | `ADSHANDLE` | Handle da tabela. |
| `pusBlockSize` | `UNSIGNED16*` | Ponteiro para receber o tamanho do bloco. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsGetMemoBlockSize` retorna o tamanho do bloco memo em bytes. Por omissão, 64 bytes.

## Exemplo

```c
UNSIGNED16 usBlockSize;
AdsGetMemoBlockSize(hTable, &usBlockSize);
// usBlockSize contém o tamanho do bloco memo
```

## Ver Também

- [AdsGetMemoLength]({{ site.baseurl }}/pt/funcoes/ads-get-memo-length/)
- [AdsGetMemoDataType]({{ site.baseurl }}/pt/funcoes/ads-get-memo-data-type/)
- [AdsGetFieldType]({{ site.baseurl }}/pt/funcoes/ads-get-field-type/)

---

[AdsGetMemoLength →]({{ site.baseurl }}/pt/funcoes/ads-get-memo-length/)
