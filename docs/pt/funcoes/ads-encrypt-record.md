---
title: AdsEncryptRecord
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-encrypt-record/
---

# AdsEncryptRecord

Encripta o registo atual.

## Sintaxe

```c
UNSIGNED32 AdsEncryptRecord(ADSHANDLE hTable);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |

## Valor de Retorno

`AE_FUNCTION_NOT_AVAILABLE` sempre (não implementado).

## Descrição

`AdsEncryptRecord` encripta o registo atual. No OpenADS, esta função não está implementada e retorna `AE_FUNCTION_NOT_AVAILABLE`.

## Ver Também

- [AdsDecryptRecord]({{ site.baseurl }}/pt/funcoes/ads-decrypt-record/)
- [AdsEncryptTable]({{ site.baseurl }}/pt/funcoes/ads-encrypt-table/)
- [AdsIsRecordEncrypted]({{ site.baseurl }}/pt/funcoes/ads-is-record-encrypted/)

---

[AdsDecryptRecord →]({{ site.baseurl }}/pt/funcoes/ads-decrypt-record/)
