---
title: AdsShowError
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-show-error/
---

# AdsShowError

Mostra uma mensagem de erro.

## Sintaxe

```c
UNSIGNED32 AdsShowError(UNSIGNED8* pucErrText);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `pucErrText` | `UNSIGNED8*` | Texto do erro a mostrar. |

## Valor de Retorno

`AE_SUCCESS` (0) sempre.

## Descrição

`AdsShowError` mostra uma mensagem de erro. No OpenADS (headless), o texto é escrito para stderr.

## Exemplo

```c
AdsShowError("Erro ao abrir tabela");
```

## Ver Também

- [AdsGetLastError]({{ site.baseurl }}/pt/funcoes/ads-get-last-error/)
- [AdsGetErrorString]({{ site.baseurl }}/pt/funcoes/ads-get-error-string/)

---

[AdsGetVersion →]({{ site.baseurl }}/pt/funcoes/ads-get-version/)
