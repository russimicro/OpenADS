---
title: AdsGetVersion
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-version/
---

# AdsGetVersion

Retorna a versão do engine.

## Sintaxe

```c
UNSIGNED32 AdsGetVersion(UNSIGNED32* pulMajor, UNSIGNED32* pulMinor,
                         UNSIGNED8*  pucLetter, UNSIGNED8* pucDesc,
                         UNSIGNED16* pusDescLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `pulMajor` | `UNSIGNED32*` | Ponteiro para receber a versão maior. |
| `pulMinor` | `UNSIGNED32*` | Ponteiro para receber a versão menor. |
| `pucLetter` | `UNSIGNED8*` | Ponteiro para receber a letra da versão. |
| `pucDesc` | `UNSIGNED8*` | Buffer para receber a descrição. |
| `pusDescLen` | `UNSIGNED16*` | Ponteiro para o tamanho do buffer. Na saída, contém o comprimento. |

## Valor de Retorno

`AE_SUCCESS` (0) sempre.

## Descrição

`AdsGetVersion` retorna informações sobre a versão do engine OpenADS.

## Exemplo

```c
UNSIGNED32 ulMajor, ulMinor;
AdsGetVersion(&ulMajor, &ulMinor, nullptr, nullptr, nullptr);
```

## Ver Também

- [AdsGetLastError]({{ site.baseurl }}/pt/funcoes/ads-get-last-error/)
- [AdsGetServerName]({{ site.baseurl }}/pt/funcoes/ads-get-server-name/)

---

[AdsGetServerName →]({{ site.baseurl }}/pt/funcoes/ads-get-server-name/)
