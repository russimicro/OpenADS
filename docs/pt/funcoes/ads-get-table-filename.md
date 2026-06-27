---
title: AdsGetTableFilename
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-table-filename/
---

# AdsGetTableFilename

Retorna o nome do arquivo da tabela.

## Sintaxe

```c
UNSIGNED32 AdsGetTableFilename(ADSHANDLE hTable, UNSIGNED16 usOption,
                               UNSIGNED8* pucBuf, UNSIGNED16* pusLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `usOption` | `UNSIGNED16` | Opção (reservada). |
| `pucBuf` | `UNSIGNED8*` | Buffer para receber o nome do arquivo. |
| `pusLen` | `UNSIGNED16*` | Ponteiro para o tamanho do buffer. Na saída, contém o comprimento. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsGetTableFilename` retorna o caminho completo do arquivo da tabela.

## Exemplo

```c
UNSIGNED8 szFile[256];
UNSIGNED16 usLen = sizeof(szFile);
AdsGetTableFilename(hTable, 0, szFile, &usLen);
// szFile contém o caminho do arquivo
```

## Ver Também

- [AdsGetTableAlias]({{ site.baseurl }}/pt/funcoes/ads-get-table-alias/)
- [AdsGetTableType]({{ site.baseurl }}/pt/funcoes/ads-get-table-type/)
- [AdsOpenTable]({{ site.baseurl }}/pt/funcoes/ads-open-table/)

---

[AdsGetTableAlias →]({{ site.baseurl }}/pt/funcoes/ads-get-table-alias/)
