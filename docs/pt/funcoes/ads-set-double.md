---
title: AdsSetDouble
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-set-double/
---

# AdsSetDouble

Define o valor de um campo numérico.

## Sintaxe

```c
UNSIGNED32 AdsSetDouble(ADSHANDLE hTable, UNSIGNED8* pucField,
                        double dValue);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela ou statement. |
| `pucField` | `UNSIGNED8*` | Nome do campo ou parâmetro SQL. |
| `dValue` | `double` | Valor numérico a definir. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsSetDouble` define o valor de um campo numérico. Se o handle for um statement SQL, o valor é armazenado como parâmetro numérico.

Para gravar o registo, é necessário chamar `AdsWriteRecord` após definir os campos.

## Exemplo

```c
AdsSetDouble(hTable, "Preco", 19.99);
AdsWriteRecord(hTable);
```

## Ver Também

- [AdsGetDouble]({{ site.baseurl }}/pt/funcoes/ads-get-double/)
- [AdsSetString]({{ site.baseurl }}/pt/funcoes/ads-set-string/)
- [AdsWriteRecord]({{ site.baseurl }}/pt/funcoes/ads-write-record/)

---

[AdsGetDouble →]({{ site.baseurl }}/pt/funcoes/ads-get-double/)
