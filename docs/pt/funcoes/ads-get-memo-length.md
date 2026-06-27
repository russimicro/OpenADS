---
title: AdsGetMemoLength
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-memo-length/
---

# AdsGetMemoLength

Retorna o comprimento do conteúdo memo.

## Sintaxe

```c
UNSIGNED32 AdsGetMemoLength(ADSHANDLE hTable, UNSIGNED8* pucField,
                            UNSIGNED32* pulLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucField` | `UNSIGNED8*` | Nome do campo memo. |
| `pulLen` | `UNSIGNED32*` | Ponteiro para receber o comprimento. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_COLUMN_NOT_FOUND` (6125) se o campo não for encontrado.

## Descrição

`AdsGetMemoLength` retorna o comprimento em caracteres do conteúdo de um campo memo.

Para tabelas remotas, o conteúdo é obtido do servidor.

## Exemplo

```c
UNSIGNED32 ulLen;
AdsGetMemoLength(hTable, "Observacoes", &ulLen);
// ulLen contém o comprimento do memo
```

## Ver Também

- [AdsGetField]({{ site.baseurl }}/pt/funcoes/ads-get-field/)
- [AdsGetString]({{ site.baseurl }}/pt/funcoes/ads-get-string/)
- [AdsGetMemoBlockSize]({{ site.baseurl }}/pt/funcoes/ads-get-memo-block-size/)

---

[AdsGetMemoDataType →]({{ site.baseurl }}/pt/funcoes/ads-get-memo-data-type/)
