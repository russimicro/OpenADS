---
title: AdsIsNull
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-is-null/
---

# AdsIsNull

Verifica se um campo é nulo.

## Sintaxe

```c
UNSIGNED32 AdsIsNull(ADSHANDLE hTable, UNSIGNED8* pucField,
                     UNSIGNED16* pbNull);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucField` | `UNSIGNED8*` | Nome do campo. |
| `pbNull` | `UNSIGNED16*` | Ponteiro para receber 1 se nulo, 0 caso contrário. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_COLUMN_NOT_FOUND` (6125) se o campo não for encontrado.

## Descrição

`AdsIsNull` verifica se um campo é nulo. Para tabelas remotas, retorna 0 (não nulo) por omissão.

## Exemplo

```c
UNSIGNED16 pbNull;
AdsIsNull(hTable, "Email", &pbNull);
// pbNull indica se o campo é nulo
```

## Ver Também

- [AdsSetNull]({{ site.baseurl }}/pt/funcoes/ads-set-null/)
- [AdsGetField]({{ site.baseurl }}/pt/funcoes/ads-get-field/)
- [AdsGetString]({{ site.baseurl }}/pt/funcoes/ads-get-string/)

---

[AdsSetNull →]({{ site.baseurl }}/pt/funcoes/ads-set-null/)
