---
title: AdsSetStringW
layout: default
parent: Referência da API
nav_order: 39
permalink: /pt/funcoes/ads-set-string-w/
---

# AdsSetStringW

Define o valor de um campo de caracteres usando string Wide (UTF-16LE).

## Sintaxe

```c
UNSIGNED32 AdsSetStringW(ADSHANDLE hTable, UNSIGNED8* pucField,
                         UNSIGNED16* pucValueW, UNSIGNED32 ulLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucField` | `UNSIGNED8*` | Nome ou índice ordinal (1-based) do campo. |
| `pucValueW` | `UNSIGNED16*` | String Wide (UTF-16LE) a ser gravada. |
| `ulLen` | `UNSIGNED32` | Número de caracteres na string Wide. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsSetStringW` grava uma string Wide (UTF-16LE) em um campo de caracteres. Isso é útil para trabalhar com dados que contêm caracteres Unicode. O campo de nome permanece ASCII.

## Exemplo

```c
UNSIGNED16 awcNome[] = { 'J', 'o', 'a', 'o', '\0' };
AdsSetStringW(hTable, "Nome", awcNome, 4);
```

## Ver Também

- [AdsSetString]({{ site.baseurl }}/pt/funcoes/ads-set-string/)
- [AdsGetStringW]({{ site.baseurl }}/pt/funcoes/ads-get-string-w/)
- [AdsGetFieldW]({{ site.baseurl }}/pt/funcoes/ads-get-field-w/)

---

[AdsSkipUnique →]({{ site.baseurl }}/pt/funcoes/ads-skip-unique/)
