---
title: AdsGetRecordLength
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-record-length/
---

# AdsGetRecordLength

Retorna o comprimento em bytes de um registo.

## Sintaxe

```c
UNSIGNED32 AdsGetRecordLength(ADSHANDLE hTable, UNSIGNED32* pulLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pulLen` | `UNSIGNED32*` | Ponteiro para receber o comprimento do registo. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsGetRecordLength` retorna o comprimento em bytes de um registo da tabela. Este valor inclui o byte de flag de eliminação e todos os campos.

## Exemplo

```c
UNSIGNED32 ulLen;
AdsGetRecordLength(hTable, &ulLen);
// ulLen contém o comprimento do registo em bytes
```

## Ver Também

- [AdsGetRecord]({{ site.baseurl }}/pt/funcoes/ads-get-record/)
- [AdsSetRecord]({{ site.baseurl }}/pt/funcoes/ads-set-record/)
- [AdsGetNumFields]({{ site.baseurl }}/pt/funcoes/ads-get-num-fields/)

---

[AdsGetRecord →]({{ site.baseurl }}/pt/funcoes/ads-get-record/)
