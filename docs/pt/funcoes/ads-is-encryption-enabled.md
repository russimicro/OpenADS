---
title: AdsIsEncryptionEnabled
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-is-encryption-enabled/
---

# AdsIsEncryptionEnabled

Verifica se a encriptação está ativa.

## Sintaxe

```c
UNSIGNED32 AdsIsEncryptionEnabled(ADSHANDLE hConnect, UNSIGNED16* pbEnabled);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `pbEnabled` | `UNSIGNED16*` | Ponteiro para receber 1 se ativa, 0 caso contrário. |

## Valor de Retorno

`AE_SUCCESS` (0) sempre.

## Descrição

`AdsIsEncryptionEnabled` verifica se a encriptação está ativa na conexão. No OpenADS, retorna sempre 0.

## Exemplo

```c
UNSIGNED16 pbEnabled;
AdsIsEncryptionEnabled(hConnect, &pbEnabled);
// pbEnabled é 0 (encriptação não suportada)
```

## Ver Também

- [AdsSetEncryptionPassword]({{ site.baseurl }}/pt/funcoes/ads-set-encryption-password/)
- [AdsEnableEncryption]({{ site.baseurl }}/pt/funcoes/ads-enable-encryption/)
- [AdsDisableEncryption]({{ site.baseurl }}/pt/funcoes/ads-disable-encryption/)

---

[AdsIsTableEncrypted →]({{ site.baseurl }}/pt/funcoes/ads-is-table-encrypted/)
