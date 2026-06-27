---
title: AdsIsRecordEncrypted
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-is-record-encrypted/
---

# AdsIsRecordEncrypted

Verifica se o registo está encriptado.

## Sintaxe

```c
UNSIGNED32 AdsIsRecordEncrypted(ADSHANDLE hTable, UNSIGNED16* pbEncrypted);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pbEncrypted` | `UNSIGNED16*` | Ponteiro para receber 1 se encriptado, 0 caso contrário. |

## Valor de Retorno

`AE_SUCCESS` (0) sempre.

## Descrição

`AdsIsRecordEncrypted` verifica se o registo atual está encriptado. No OpenADS, retorna sempre 0.

## Exemplo

```c
UNSIGNED16 pbEncrypted;
AdsIsRecordEncrypted(hTable, &pbEncrypted);
// pbEncrypted é 0 (registo não encriptado)
```

## Ver Também

- [AdsEncryptRecord]({{ site.baseurl }}/pt/funcoes/ads-encrypt-record/)
- [AdsDecryptRecord]({{ site.baseurl }}/pt/funcoes/ads-decrypt-record/)
- [AdsIsTableEncrypted]({{ site.baseurl }}/pt/funcoes/ads-is-table-encrypted/)

---

[AdsEncryptTable →]({{ site.baseurl }}/pt/funcoes/ads-encrypt-table/)
