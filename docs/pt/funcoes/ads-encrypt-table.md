---
title: AdsEncryptTable
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-encrypt-table/
---

# AdsEncryptTable

Encripta uma tabela.

## Sintaxe

```c
UNSIGNED32 AdsEncryptTable(ADSHANDLE hTable);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_FUNCTION_NOT_AVAILABLE` se a encriptação não estiver configurada.

## Descrição

`AdsEncryptTable` encripta uma tabela CDX no local usando AES-256-CTR. Requer que `AdsSetEncryptionPassword` tenha sido chamado na conexão.

## Exemplo

```c
AdsSetEncryptionPassword(hConnect, "minha_chave");
AdsEncryptTable(hTable);
```

## Ver Também

- [AdsDecryptTable]({{ site.baseurl }}/pt/funcoes/ads-decrypt-table/)
- [AdsSetEncryptionPassword]({{ site.baseurl }}/pt/funcoes/ads-set-encryption-password/)
- [AdsEncryptRecord]({{ site.baseurl }}/pt/funcoes/ads-encrypt-record/)

---

[AdsDecryptTable →]({{ site.baseurl }}/pt/funcoes/ads-decrypt-table/)
