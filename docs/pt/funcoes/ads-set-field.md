---
title: AdsSetField
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-set-field/
---

# AdsSetField

Define o valor de um campo como texto.

## Sintaxe

```c
UNSIGNED32 AdsSetField(ADSHANDLE hObj, UNSIGNED8* pId, UNSIGNED8* pucBuf,
                       UNSIGNED32 ulLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hObj` | `ADSHANDLE` | Handle da tabela ou índice. |
| `pId` | `UNSIGNED8*` | Nome do campo ou ordinal (via ADSFIELD). |
| `pucBuf` | `UNSIGNED8*` | Buffer com o valor a definir. |
| `ulLen` | `UNSIGNED32` | Comprimento do valor. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsSetField` define o valor de um campo como texto. A função converte internamente para o tipo apropriado do campo.

Para gravar o registo, é necessário chamar `AdsWriteRecord` após definir os campos.

## Exemplo

```c
AdsSetField(hTable, "Nome", "João Silva", 10);
AdsWriteRecord(hTable);
```

## Ver Também

- [AdsGetField]({{ site.baseurl }}/pt/funcoes/ads-get-field/)
- [AdsSetString]({{ site.baseurl }}/pt/funcoes/ads-set-string/)
- [AdsWriteRecord]({{ site.baseurl }}/pt/funcoes/ads-write-record/)

---

[AdsWriteRecord →]({{ site.baseurl }}/pt/funcoes/ads-write-record/)
