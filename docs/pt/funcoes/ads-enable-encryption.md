---
title: AdsEnableEncryption
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-enable-encryption/
---

# AdsEnableEncryption

Ativa a encriptação.

## Sintaxe

```c
UNSIGNED32 AdsEnableEncryption(ADSHANDLE hConnect, UNSIGNED8* pucPassword);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `pucPassword` | `UNSIGNED8*` | Palavra-passe de encriptação. |

## Valor de Retorno

`AE_FUNCTION_NOT_AVAILABLE` sempre (não implementado).

## Descrição

`AdsEnableEncryption` ativa a encriptação para a conexão. No OpenADS, esta função não está implementada.

## Ver Também

- [AdsDisableEncryption]({{ site.baseurl }}/pt/funcoes/ads-disable-encryption/)
- [AdsSetEncryptionPassword]({{ site.baseurl }}/pt/funcoes/ads-set-encryption-password/)
- [AdsEncryptTable]({{ site.baseurl }}/pt/funcoes/ads-encrypt-table/)

---

[AdsDisableEncryption →]({{ site.baseurl }}/pt/funcoes/ads-disable-encryption/)
