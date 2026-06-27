---
title: AdsSetEncryptionPassword
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-set-encryption-password/
---

# AdsSetEncryptionPassword

Define a palavra-passe de encriptação.

## Sintaxe

```c
UNSIGNED32 AdsSetEncryptionPassword(ADSHANDLE hConnect,
                                    UNSIGNED8* pucPassword);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `pucPassword` | `UNSIGNED8*` | Palavra-passe de encriptação. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INVALID_CONNECTION_HANDLE` se a conexão for inválida.

## Descrição

`AdsSetEncryptionPassword` define a palavra-passe de encriptação para a conexão. Esta chave é usada para encriptar/desencriptar tabelas.

## Exemplo

```c
AdsSetEncryptionPassword(hConnect, "minha_chave_secreta");
AdsEncryptTable(hTable);
```

## Ver Também

- [AdsEncryptTable]({{ site.baseurl }}/pt/funcoes/ads-encrypt-table/)
- [AdsDecryptTable]({{ site.baseurl }}/pt/funcoes/ads-decrypt-table/)
- [AdsIsEncryptionEnabled]({{ site.baseurl }}/pt/funcoes/ads-is-encryption-enabled/)

---

[AdsEnableEncryption →]({{ site.baseurl }}/pt/funcoes/ads-enable-encryption/)
