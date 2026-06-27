---
title: AdsConnect26
layout: default
parent: Referência da API
nav_order: 17
permalink: /pt/funcoes/ads-connect-26/
---

# AdsConnect26

Estabelece uma conexão com o servidor (versão 2.6).

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsConnect26(UNSIGNED8* pucServer,
                                   UNSIGNED16 usServerType,
                                   ADSHANDLE* phConnect);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `pucServer` | `UNSIGNED8*` | Caminho do servidor. |
| `usServerType` | `UNSIGNED16` | Tipo do servidor (reservado). |
| `phConnect` | `ADSHANDLE*` | Ponteiro para receber o handle da conexão. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsConnect26` é uma versão simplificada de `AdsConnect60`, compatível com a API da versão 2.6. Não suporta autenticação nem protocolos TLS.

## Exemplo

```c
ADSHANDLE hConnect;
AdsConnect26("C:\\dados", ADS_LOCAL_SERVER, &hConnect);
```

## Ver Também

- [AdsConnect60]({{ site.baseurl }}/pt/funcoes/ads-connect-60/)
- [AdsDisconnect]({{ site.baseurl }}/pt/funcoes/ads-disconnect/)

---

[AdsContinue →]({{ site.baseurl }}/pt/funcoes/ads-continue/)
