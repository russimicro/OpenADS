---
title: AdsGetTableAlias
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-table-alias/
---

# AdsGetTableAlias

Retorna o alias da tabela.

## Sintaxe

```c
UNSIGNED32 AdsGetTableAlias(ADSHANDLE hTable, UNSIGNED8* p, UNSIGNED16* l);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `p` | `UNSIGNED8*` | Buffer para receber o alias. |
| `l` | `UNSIGNED16*` | Ponteiro para o tamanho do buffer. Na saída, contém o comprimento. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsGetTableAlias` retorna o alias da tabela (normalmente o nome do arquivo sem extensão).

## Exemplo

```c
UNSIGNED8 szAlias[128];
UNSIGNED16 usLen = sizeof(szAlias);
AdsGetTableAlias(hTable, szAlias, &usLen);
// szAlias contém o alias da tabela
```

## Ver Também

- [AdsGetTableFilename]({{ site.baseurl }}/pt/funcoes/ads-get-table-filename/)
- [AdsGetTableType]({{ site.baseurl }}/pt/funcoes/ads-get-table-type/)
- [AdsOpenTable]({{ site.baseurl }}/pt/funcoes/ads-open-table/)

---

[AdsGetTableCharType →]({{ site.baseurl }}/pt/funcoes/ads-get-table-char-type/)
