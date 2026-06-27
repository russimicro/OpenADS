---
title: AdsDisableEncryption
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-disable-encryption/
---

# AdsDisableEncryption

Desativa a encriptação.

## Sintaxe

```c
UNSIGNED32 AdsDisableEncryption(ADSHANDLE hConnect);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |

## Valor de Retorno

`AE_SUCCESS` (0) sempre.

## Descrição

`AdsDisableEncryption` desativa a encriptação para a conexão.

## Exemplo

```c
AdsDisableEncryption(hConnect);
```

## Ver Também

- [AdsEnableEncryption]({{ site.baseurl }}/pt/funcoes/ads-enable-encryption/)
- [AdsSetEncryptionPassword]({{ site.baseurl }}/pt/funcoes/ads-set-encryption-password/)
- [AdsIsEncryptionEnabled]({{ site.baseurl }}/pt/funcoes/ads-is-encryption-enabled/)

---

[AdsCreateSavepoint →]({{ site.baseurl }}/pt/funcoes/ads-create-savepoint/)
