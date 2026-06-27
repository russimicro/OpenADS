---
title: AdsGetLastAutoinc
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-last-autoinc/
---

# AdsGetLastAutoinc

Retorna o último valor autoincremento gerado.

## Sintaxe

```c
UNSIGNED32 AdsGetLastAutoinc(ADSHANDLE hTable, UNSIGNED32* pulValue);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pulValue` | `UNSIGNED32*` | Ponteiro para receber o último valor autoinc. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsGetLastAutoinc` retorna o último valor gerado para um campo autoincremento. Para tabelas que não suportam autoinc, retorna 0.

## Exemplo

```c
UNSIGNED32 ulLastAuto;
AdsGetLastAutoinc(hTable, &ulLastAuto);
// ulLastAuto contém o último valor autoinc
```

## Ver Também

- [AdsGetFieldType]({{ site.baseurl }}/pt/funcoes/ads-get-field-type/)
- [AdsCreateTable]({{ site.baseurl }}/pt/funcoes/ads-create-table/)
- [AdsRestructureTable]({{ site.baseurl }}/pt/funcoes/ads-restructure-table/)

---

[AdsIsEncryptionEnabled →]({{ site.baseurl }}/pt/funcoes/ads-is-encryption-enabled/)
