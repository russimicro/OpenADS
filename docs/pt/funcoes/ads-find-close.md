---
title: AdsFindClose
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-find-close/
---

# AdsFindClose

Fecha uma operação de procura de tabelas.

## Sintaxe

```c
UNSIGNED32 AdsFindClose(ADSHANDLE hConnect, ADSHANDLE hFind);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `hFind` | `ADSHANDLE` | Handle de procura. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INVALID_CONNECTION_HANDLE` se a conexão for inválida.

## Descrição

`AdsFindClose` fecha uma operação de procura de tabelas e liberta o handle.

## Exemplo

```c
AdsFindClose(hConnect, hFind);
```

## Ver Também

- [AdsFindFirstTable]({{ site.baseurl }}/pt/funcoes/ads-find-first-table/)
- [AdsFindNextTable]({{ site.baseurl }}/pt/funcoes/ads-find-next-table/)
- [AdsOpenTable]({{ site.baseurl }}/pt/funcoes/ads-open-table/)

---

[AdsCheckExistence →]({{ site.baseurl }}/pt/funcoes/ads-check-existence/)
