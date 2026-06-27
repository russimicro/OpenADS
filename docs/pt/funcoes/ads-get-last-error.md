---
title: AdsGetLastError
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-last-error/
---

# AdsGetLastError

Retorna o último erro ocorrido.

## Sintaxe

```c
UNSIGNED32 AdsGetLastError(UNSIGNED32* pulCode, UNSIGNED8* pucBuf,
                           UNSIGNED16* pusBufLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `pulCode` | `UNSIGNED32*` | Ponteiro para receber o código do erro. |
| `pucBuf` | `UNSIGNED8*` | Buffer para receber a mensagem de erro. |
| `pusBufLen` | `UNSIGNED16*` | Ponteiro para o tamanho do buffer. Na saída, contém o comprimento. |

## Valor de Retorno

`AE_SUCCESS` (0) sempre.

## Descrição

`AdsGetLastError` retorna o código e a mensagem do último erro ocorrido na sessão atual.

## Exemplo

```c
UNSIGNED32 ulCode;
UNSIGNED8 szMsg[256];
UNSIGNED16 usLen = sizeof(szMsg);
AdsGetLastError(&ulCode, szMsg, &usLen);
// ulCode contém o código do erro
// szMsg contém a mensagem
```

## Ver Também

- [AdsGetErrorString]({{ site.baseurl }}/pt/funcoes/ads-get-error-string/)
- [AdsShowError]({{ site.baseurl }}/pt/funcoes/ads-show-error/)

---

[AdsGetErrorString →]({{ site.baseurl }}/pt/funcoes/ads-get-error-string/)
