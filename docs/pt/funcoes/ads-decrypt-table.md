---
title: AdsDecryptTable
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-decrypt-table/
---

# AdsDecryptTable

Desencripta uma tabela.

## Sintaxe

```c
UNSIGNED32 AdsDecryptTable(ADSHANDLE hTable);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |

## Valor de Retorno

`AE_FUNCTION_NOT_AVAILABLE` sempre (não implementado).

## Descrição

`AdsDecryptTable` desencripta uma tabela. No OpenADS, esta função não está implementada e retorna `AE_FUNCTION_NOT_AVAILABLE`.

## Ver Também

- [AdsEncryptTable]({{ site.baseurl }}/pt/funcoes/ads-encrypt-table/)
- [AdsSetEncryptionPassword]({{ site.baseurl }}/pt/funcoes/ads-set-encryption-password/)
- [AdsIsTableEncrypted]({{ site.baseurl }}/pt/funcoes/ads-is-table-encrypted/)

---

[AdsEncryptRecord →]({{ site.baseurl }}/pt/funcoes/ads-encrypt-record/)
