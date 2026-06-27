---
title: AdsGetNumFields
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-num-fields/
---

# AdsGetNumFields

Retorna o número de campos da tabela.

## Sintaxe

```c
UNSIGNED32 AdsGetNumFields(ADSHANDLE hTable, UNSIGNED16* pusFields);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pusFields` | `UNSIGNED16*` | Ponteiro para receber o número de campos. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsGetNumFields` retorna o número total de campos da tabela. Para tabelas remotas, a descrição da tabela é cacheada após a primeira chamada.

Se a tabela tiver uma projeção ativa (SELECT com colunas específicas), a função retorna o número de campos da projeção.

## Exemplo

```c
UNSIGNED16 usFields;
AdsGetNumFields(hTable, &usFields);
// usFields contém o número de campos
```

## Ver Também

- [AdsGetFieldName]({{ site.baseurl }}/pt/funcoes/ads-get-field-name/)
- [AdsGetFieldType]({{ site.baseurl }}/pt/funcoes/ads-get-field-type/)
- [AdsGetFieldLength]({{ site.baseurl }}/pt/funcoes/ads-get-field-length/)

---

[AdsGetFieldName →]({{ site.baseurl }}/pt/funcoes/ads-get-field-name/)
