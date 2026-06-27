---
title: AdsGetLastTableUpdate
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-last-table-update/
---

# AdsGetLastTableUpdate

Retorna a data da última atualização da tabela.

## Sintaxe

```c
UNSIGNED32 AdsGetLastTableUpdate(ADSHANDLE hTable, UNSIGNED8* pucDate,
                                 UNSIGNED16* pusLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucDate` | `UNSIGNED8*` | Buffer para receber a data. |
| `pusLen` | `UNSIGNED16*` | Ponteiro para o tamanho do buffer. Na saída, contém o comprimento. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsGetLastTableUpdate` retorna a data da última atualização da tabela no formato definido por `AdsSetDateFormat`.

## Exemplo

```c
UNSIGNED8 szDate[32];
UNSIGNED16 usLen = sizeof(szDate);
AdsGetLastTableUpdate(hTable, szDate, &usLen);
// szDate contém a data da última atualização
```

## Ver Também

- [AdsGetDateFormat]({{ site.baseurl }}/pt/funcoes/ads-get-date-format/)
- [AdsSetDateFormat]({{ site.baseurl }}/pt/funcoes/ads-set-date-format/)
- [AdsGetTableType]({{ site.baseurl }}/pt/funcoes/ads-get-table-type/)

---

[AdsGetLastAutoinc →]({{ site.baseurl }}/pt/funcoes/ads-get-last-autoinc/)
