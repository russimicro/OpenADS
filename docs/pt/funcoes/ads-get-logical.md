---
title: AdsGetLogical
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-logical/
---

# AdsGetLogical

Retorna o valor de um campo lógico.

## Sintaxe

```c
UNSIGNED32 AdsGetLogical(ADSHANDLE hTable, UNSIGNED8* pucField, UNSIGNED16* pb);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucField` | `UNSIGNED8*` | Nome do campo ou ordinal (via ADSFIELD). |
| `pb` | `UNSIGNED16*` | Ponteiro para receber o valor lógico (1 = verdadeiro, 0 = falso). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_COLUMN_NOT_FOUND` (6125) se o campo não for encontrado.

## Descrição

`AdsGetLogical` retorna o valor de um campo lógico. O valor retornado é 1 para verdadeiro (T, Y, 1) e 0 para falso.

## Exemplo

```c
UNSIGNED16 bAtivo;
AdsGetLogical(hTable, "Ativo", &bAtivo);
// bAtivo contém 1 (verdadeiro) ou 0 (falso)
```

## Ver Também

- [AdsSetLogical]({{ site.baseurl }}/pt/funcoes/ads-set-logical/)
- [AdsGetField]({{ site.baseurl }}/pt/funcoes/ads-get-field/)
- [AdsGetString]({{ site.baseurl }}/pt/funcoes/ads-get-string/)

---

[AdsGetBookmark →]({{ site.baseurl }}/pt/funcoes/ads-get-bookmark/)
