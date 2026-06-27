---
title: AdsIsEmpty
layout: default
parent: Referência da API
nav_order: 10
permalink: /pt/funcoes/ads-is-empty/
---

# AdsIsEmpty

Verifica se um campo está vazio.

## Sintaxe

```c
UNSIGNED32 AdsIsEmpty(ADSHANDLE hTable, UNSIGNED8* pucField,
                      UNSIGNED16* pbEmpty);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucField` | `UNSIGNED8*` | Nome ou índice ordinal (1-based) do campo. |
| `pbEmpty` | `UNSIGNED16*` | Ponteiro para variável que recebe o resultado (1=vazio, 0=preenchido). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsIsEmpty` verifica se o valor do campo especificado está vazio (consiste apenas em espaços ou bytes zero). Útil para campos nullable ou campos que podem conter valores vazios.

## Exemplo

```c
UNSIGNED16 bEmpty;
AdsIsEmpty(hTable, "Telefone", &bEmpty);
if (bEmpty) {
    // Campo está vazio
}
```

## Ver Também

- [AdsIsNull]({{ site.baseurl }}/pt/funcoes/ads-is-null/)
- [AdsGetField]({{ site.baseurl }}/pt/funcoes/ads-get-field/)
- [AdsSetEmpty]({{ site.baseurl }}/pt/funcoes/ads-set-empty/)

---

[AdsIsExprValid →]({{ site.baseurl }}/pt/funcoes/ads-is-expr-valid/)
