---
title: AdsSetString
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-set-string/
---

# AdsSetString

Define o valor de um campo como string.

## Sintaxe

```c
UNSIGNED32 AdsSetString(ADSHANDLE hTable, UNSIGNED8* pucField,
                        UNSIGNED8* pucValue, UNSIGNED32 ulLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela ou statement. |
| `pucField` | `UNSIGNED8*` | Nome do campo ou parâmetro SQL. |
| `pucValue` | `UNSIGNED8*` | Valor a definir. |
| `ulLen` | `UNSIGNED32` | Comprimento do valor. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsSetString` define o valor de um campo como string. Se o handle for um statement SQL, o valor é armazenado como parâmetro com aspas simples escapadas.

Para gravar o registo, é necessário chamar `AdsWriteRecord` após definir os campos.

## Exemplo

```c
AdsSetString(hTable, "Nome", "João Silva", 10);
AdsWriteRecord(hTable);
```

## Ver Também

- [AdsGetString]({{ site.baseurl }}/pt/funcoes/ads-get-string/)
- [AdsSetField]({{ site.baseurl }}/pt/funcoes/ads-set-field/)
- [AdsWriteRecord]({{ site.baseurl }}/pt/funcoes/ads-write-record/)

---

[AdsSetDouble →]({{ site.baseurl }}/pt/funcoes/ads-set-double/)
