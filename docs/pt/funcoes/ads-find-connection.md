---
title: AdsFindConnection
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-find-connection/
---

# AdsFindConnection

Encontra uma conexão pelo servidor.

## Sintaxe

```c
UNSIGNED32 AdsFindConnection(UNSIGNED8* pucServer, ADSHANDLE* phConnect);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `pucServer` | `UNSIGNED8*` | Servidor. |
| `phConnect` | `ADSHANDLE*` | Ponteiro para receber o handle da conexão. |

## Valor de Retorno

`AE_NO_CONNECTION` sempre.

## Descrição

`AdsFindConnection` tenta encontrar uma conexão pelo servidor. No OpenADS, retorna sempre `AE_NO_CONNECTION`.

## Ver Também

- [AdsConnect]({{ site.baseurl }}/pt/funcoes/ads-connect/)
- [AdsConnect60]({{ site.baseurl }}/pt/funcoes/ads-connect-60/)
- [AdsFindConnection25]({{ site.baseurl }}/pt/funcoes/ads-find-connection-25/)

---

[AdsGetTableHandle25 →]({{ site.baseurl }}/pt/funcoes/ads-get-table-handle-25/)
