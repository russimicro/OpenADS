---
title: AdsSetNull
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-set-null/
---

# AdsSetNull

Define um campo como nulo.

## Sintaxe

```c
UNSIGNED32 AdsSetNull(ADSHANDLE hObj, UNSIGNED8* pId);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hObj` | `ADSHANDLE` | Handle da tabela ou statement. |
| `pId` | `UNSIGNED8*` | Nome do campo ou ordinal. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsSetNull` define um campo como nulo. Em tabelas DBF, armazena vazio (não SQL NULL).

## Exemplo

```c
AdsSetNull(hTable, "Email");
AdsWriteRecord(hTable);
```

## Ver Também

- [AdsIsNull]({{ site.baseurl }}/pt/funcoes/ads-is-null/)
- [AdsSetEmpty]({{ site.baseurl }}/pt/funcoes/ads-set-empty/)
- [AdsSetField]({{ site.baseurl }}/pt/funcoes/ads-set-field/)

---

[AdsSetEmpty →]({{ site.baseurl }}/pt/funcoes/ads-set-empty/)
