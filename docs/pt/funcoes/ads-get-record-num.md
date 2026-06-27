---
title: AdsGetRecordNum
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-record-num/
---

# AdsGetRecordNum

Retorna o número do registo atual.

## Sintaxe

```c
UNSIGNED32 AdsGetRecordNum(ADSHANDLE hTable, UNSIGNED16 bFilterOption,
                           UNSIGNED32* pulRecordNum);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `bFilterOption` | `UNSIGNED16` | Opção de filtro (reservada, usar 0). |
| `pulRecordNum` | `UNSIGNED32*` | Ponteiro para receber o número do registo. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsGetRecordNum` retorna o número do registo atual (1-based). Para tabelas remotas, o número do registo é retornado do cache de linha, sem comunicação adicional com o servidor.

## Exemplo

```c
UNSIGNED32 ulRecno;
AdsGetRecordNum(hTable, 0, &ulRecno);
// ulRecno contém o número do registo atual
```

## Ver Também

- [AdsGotoRecord]({{ site.baseurl }}/pt/funcoes/ads-goto-record/)
- [AdsGetRecordCount]({{ site.baseurl }}/pt/funcoes/ads-get-record-count/)
- [AdsGetBookmark]({{ site.baseurl }}/pt/funcoes/ads-get-bookmark/)

---

[AdsGetRecordLength →]({{ site.baseurl }}/pt/funcoes/ads-get-record-length/)
