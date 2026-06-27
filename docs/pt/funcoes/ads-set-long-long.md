---
title: AdsSetLongLong
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-set-long-long/
---

# AdsSetLongLong

Define o valor de um campo como inteiro de 64 bits.

## Sintaxe

```c
UNSIGNED32 AdsSetLongLong(ADSHANDLE hTable, UNSIGNED8* pucField,
                          std::int64_t llValue);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela ou statement. |
| `pucField` | `UNSIGNED8*` | Nome do campo ou parâmetro SQL. |
| `llValue` | `std::int64_t` | Valor inteiro de 64 bits a definir. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsSetLongLong` define o valor de um campo como inteiro de 64 bits. Internamente, a função converte para double antes de armazenar.

## Exemplo

```c
AdsSetLongLong(hTable, "CodigoGrande", 123456789012345LL);
AdsWriteRecord(hTable);
```

## Ver Também

- [AdsGetLongLong]({{ site.baseurl }}/pt/funcoes/ads-get-long-long/)
- [AdsSetDouble]({{ site.baseurl }}/pt/funcoes/ads-set-double/)
- [AdsWriteRecord]({{ site.baseurl }}/pt/funcoes/ads-write-record/)

---

[AdsSetLogical →]({{ site.baseurl }}/pt/funcoes/ads-set-logical/)
