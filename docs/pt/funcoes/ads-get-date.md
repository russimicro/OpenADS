---
title: AdsGetDate
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-date/
---

# AdsGetDate

Retorna o valor de um campo de data como uma string formatada.

## Sintaxe

```c
UNSIGNED32 AdsGetDate(ADSHANDLE hObj, UNSIGNED8* pucFldId,
                      UNSIGNED8* pucBuf, UNSIGNED16* pusLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hObj` | `ADSHANDLE` | Handle da tabela ou cursor. |
| `pucFldId` | `UNSIGNED8*` | Nome ou índice ordinal (1-based) do campo. |
| `pucBuf` | `UNSIGNED8*` | Buffer de saída para a string de data. |
| `pusLen` | `UNSIGNED16*` | Tamanho do buffer (entrada) e bytes escritos (saída). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsGetDate` obtém o valor de um campo de data e o retorna como uma string formatada de acordo com o formato de data ativo (definido via `AdsSetDateFormat`). O formato padrão é "MM/DD/AA".

## Exemplo

```c
UNSIGNED8 aucDate[32];
UNSIGNED16 usLen = sizeof(aucDate);
AdsGetDate(hTable, "DataNasc", aucDate, &usLen);
// aucDate contém "12/25/2025"
```

## Ver Também

- [AdsSetDateFormat]({{ site.baseurl }}/pt/funcoes/ads-set-date-format/)
- [AdsGetDateFormat]({{ site.baseurl }}/pt/funcoes/ads-get-date-format/)
- [AdsGetDate60]({{ site.baseurl }}/pt/funcoes/ads-get-date-60/)

---

[AdsGetDateFormat60 →]({{ site.baseurl }}/pt/funcoes/ads-get-date-format-60/)
