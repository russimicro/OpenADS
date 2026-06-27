---
title: AdsSetLogical
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-set-logical/
---

# AdsSetLogical

Define o valor de um campo lógico.

## Sintaxe

```c
UNSIGNED32 AdsSetLogical(ADSHANDLE hTable, UNSIGNED8* pucField,
                         UNSIGNED16 bValue);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela ou statement. |
| `pucField` | `UNSIGNED8*` | Nome do campo ou parâmetro SQL. |
| `bValue` | `UNSIGNED16` | Valor lógico (1 = verdadeiro, 0 = falso). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsSetLogical` define o valor de um campo lógico. Em campos DBF, o valor é armazenado como 'T' (verdadeiro) ou 'F' (falso).

Para statements SQL, o valor é armazenado como 1 ou 0.

## Exemplo

```c
AdsSetLogical(hTable, "Ativo", 1);
AdsWriteRecord(hTable);
```

## Ver Também

- [AdsGetLogical]({{ site.baseurl }}/pt/funcoes/ads-get-logical/)
- [AdsSetString]({{ site.baseurl }}/pt/funcoes/ads-set-string/)
- [AdsWriteRecord]({{ site.baseurl }}/pt/funcoes/ads-write-record/)

---

[AdsGetLogical →]({{ site.baseurl }}/pt/funcoes/ads-get-logical/)
