---
title: AdsSetRecord
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-set-record/
---

# AdsSetRecord

Define o conteúdo bruto do registo atual.

## Sintaxe

```c
UNSIGNED32 AdsSetRecord(ADSHANDLE hTable, UNSIGNED8* pucRecord,
                        UNSIGNED32 ulLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucRecord` | `UNSIGNED8*` | Buffer com o conteúdo do registo. |
| `ulLen` | `UNSIGNED32` | Comprimento do buffer. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_FUNCTION_NOT_AVAILABLE` para tabelas remotas.

## Descrição

`AdsSetRecord` define o conteúdo bruto (binário) do registo atual. O buffer deve conter o registo no formato interno do DBF.

**Nota:** Esta função não está disponível para tabelas remotas.

Para gravar as alterações, é necessário chamar `AdsWriteRecord`.

## Exemplo

```c
AdsSetRecord(hTable, buffer, ulLen);
AdsWriteRecord(hTable);
```

## Ver Também

- [AdsGetRecord]({{ site.baseurl }}/pt/funcoes/ads-get-record/)
- [AdsWriteRecord]({{ site.baseurl }}/pt/funcoes/ads-write-record/)
- [AdsGetRecordLength]({{ site.baseurl }}/pt/funcoes/ads-get-record-length/)

---

[AdsGetString →]({{ site.baseurl }}/pt/funcoes/ads-get-string/)
