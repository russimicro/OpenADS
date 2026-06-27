---
title: AdsGetErrorString
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-error-string/
---

# AdsGetErrorString

Retorna a descrição de um código de erro.

## Sintaxe

```c
UNSIGNED32 AdsGetErrorString(UNSIGNED32 ulErrCode, UNSIGNED8* pucBuf,
                             UNSIGNED16* pusLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `ulErrCode` | `UNSIGNED32` | Código do erro. |
| `pucBuf` | `UNSIGNED8*` | Buffer para receber a descrição. |
| `pusLen` | `UNSIGNED16*` | Ponteiro para o tamanho do buffer. Na saída, contém o comprimento. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o código for desconhecido.

## Descrição

`AdsGetErrorString` retorna a descrição textual de um código de erro.

## Exemplo

```c
UNSIGNED8 szMsg[256];
UNSIGNED16 usLen = sizeof(szMsg);
AdsGetErrorString(AE_COLUMN_NOT_FOUND, szMsg, &usLen);
// szMsg contém "Column not found"
```

## Ver Também

- [AdsGetLastError]({{ site.baseurl }}/pt/funcoes/ads-get-last-error/)
- [AdsShowError]({{ site.baseurl }}/pt/funcoes/ads-show-error/)

---

[AdsShowError →]({{ site.baseurl }}/pt/funcoes/ads-show-error/)
