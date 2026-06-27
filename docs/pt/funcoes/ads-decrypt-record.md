---
title: AdsDecryptRecord
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-decrypt-record/
---

# AdsDecryptRecord

Desencripta o registo atual.

## Sintaxe

```c
UNSIGNED32 AdsDecryptRecord(ADSHANDLE hTable);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |

## Valor de Retorno

`AE_FUNCTION_NOT_AVAILABLE` sempre (não implementado).

## Descrição

`AdsDecryptRecord` desencripta o registo atual. No OpenADS, esta função não está implementada e retorna `AE_FUNCTION_NOT_AVAILABLE`.

## Ver Também

- [AdsEncryptRecord]({{ site.baseurl }}/pt/funcoes/ads-encrypt-record/)
- [AdsDecryptTable]({{ site.baseurl }}/pt/funcoes/ads-decrypt-table/)
- [AdsIsRecordEncrypted]({{ site.baseurl }}/pt/funcoes/ads-is-record-encrypted/)

---

[AdsSetEncryptionPassword →]({{ site.baseurl }}/pt/funcoes/ads-set-encryption-password/)
