---
title: AdsIsTableEncrypted
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-is-table-encrypted/
---

# AdsIsTableEncrypted

Verifica se a tabela está encriptada.

## Sintaxe

```c
UNSIGNED32 AdsIsTableEncrypted(ADSHANDLE hTable, UNSIGNED16* pbEncrypted);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pbEncrypted` | `UNSIGNED16*` | Ponteiro para receber 1 se encriptada, 0 caso contrário. |

## Valor de Retorno

`AE_SUCCESS` (0) sempre.

## Descrição

`AdsIsTableEncrypted` verifica se a tabela está encriptada. No OpenADS, retorna sempre 0.

## Exemplo

```c
UNSIGNED16 pbEncrypted;
AdsIsTableEncrypted(hTable, &pbEncrypted);
// pbEncrypted é 0 (tabela não encriptada)
```

## Ver Também

- [AdsEncryptTable]({{ site.baseurl }}/pt/funcoes/ads-encrypt-table/)
- [AdsDecryptTable]({{ site.baseurl }}/pt/funcoes/ads-decrypt-table/)
- [AdsIsRecordEncrypted]({{ site.baseurl }}/pt/funcoes/ads-is-record-encrypted/)

---

[AdsIsRecordEncrypted →]({{ site.baseurl }}/pt/funcoes/ads-is-record-encrypted/)
