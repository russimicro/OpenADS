---
title: AdsGetTableType
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-table-type/
---

# AdsGetTableType

Retorna o tipo da tabela.

## Sintaxe

```c
UNSIGNED32 AdsGetTableType(ADSHANDLE hTable, UNSIGNED16* pusType);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pusType` | `UNSIGNED16*` | Ponteiro para receber o tipo da tabela. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsGetTableType` retorna o tipo da tabela:
- `ADS_CDX` (2) - DBF com CDX
- `ADS_NTX` (1) - DBF com NTX
- `ADS_ADT` (3) - Advantage Database Table
- `ADS_VFP` (4) - Visual FoxPro

## Exemplo

```c
UNSIGNED16 usType;
AdsGetTableType(hTable, &usType);
// usType indica o tipo da tabela
```

## Ver Também

- [AdsGetTableFilename]({{ site.baseurl }}/pt/funcoes/ads-get-table-filename/)
- [AdsGetTableAlias]({{ site.baseurl }}/pt/funcoes/ads-get-table-alias/)
- [AdsOpenTable]({{ site.baseurl }}/pt/funcoes/ads-open-table/)

---

[AdsGetTableFilename →]({{ site.baseurl }}/pt/funcoes/ads-get-table-filename/)
