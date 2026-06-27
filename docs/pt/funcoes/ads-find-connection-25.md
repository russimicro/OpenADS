---
title: AdsFindConnection25
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-find-connection-25/
---

# AdsFindConnection25

Encontra uma conexão pelo caminho (versão 2.5).

## Sintaxe

```c
UNSIGNED32 AdsFindConnection25(UNSIGNED8* pucFullPath,
                               ADSHANDLE* phConnect);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `pucFullPath` | `UNSIGNED8*` | Caminho completo. |
| `phConnect` | `ADSHANDLE*` | Ponteiro para receber o handle da conexão. |

## Valor de Retorno

`AE_NO_CONNECTION` sempre.

## Descrição

`AdsFindConnection25` tenta encontrar uma conexão pelo caminho. No OpenADS, retorna sempre `AE_NO_CONNECTION`.

## Ver Também

- [AdsConnect]({{ site.baseurl }}/pt/funcoes/ads-connect/)
- [AdsConnect60]({{ site.baseurl }}/pt/funcoes/ads-connect-60/)
- [AdsFindConnection]({{ site.baseurl }}/pt/funcoes/ads-find-connection/)

---

[AdsFindConnection →]({{ site.baseurl }}/pt/funcoes/ads-find-connection/)
