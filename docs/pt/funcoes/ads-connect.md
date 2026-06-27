---
title: AdsConnect
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-connect/
---

# AdsConnect

Estabelece uma conexão com o servidor.

## Sintaxe

```c
UNSIGNED32 AdsConnect(UNSIGNED8* pucServer, ADSHANDLE* phConnect);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `pucServer` | `UNSIGNED8*` | Caminho do diretório ou URI do servidor. |
| `phConnect` | `ADSHANDLE*` | Ponteiro para receber o handle da conexão. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se a conexão falhar.

## Descrição

`AdsConnect` estabelece uma conexão com um servidor OpenADS. Se o caminho for local, cria uma conexão local. Se for uma URI `tcp://`, conecta-se a um servidor remoto.

Esta função é equivalente a `AdsConnect60` com `ADS_LOCAL_SERVER`.

## Exemplo

```c
ADSHANDLE hConnect;
AdsConnect("/caminho/dados", &hConnect);
// ou para servidor remoto
AdsConnect("tcp://192.168.1.100:16262//caminho/dados", &hConnect);
```

## Ver Também

- [AdsConnect60]({{ site.baseurl }}/pt/funcoes/ads-connect-60/)
- [AdsDisconnect]({{ site.baseurl }}/pt/funcoes/ads-disconnect/)
- [AdsOpenTable]({{ site.baseurl }}/pt/funcoes/ads-open-table/)

---

[AdsConnect60 →]({{ site.baseurl }}/pt/funcoes/ads-connect-60/)
