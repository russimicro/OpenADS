---
title: AdsDisconnect
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-disconnect/
---

# AdsDisconnect

Termina uma conexão.

## Sintaxe

```c
UNSIGNED32 AdsDisconnect(ADSHANDLE hConnect);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsDisconnect` termina uma conexão e liberta todos os recursos associados. Tabelas abertas através da conexão são fechadas automaticamente.

## Exemplo

```c
AdsDisconnect(hConnect);
```

## Ver Também

- [AdsConnect]({{ site.baseurl }}/pt/funcoes/ads-connect/)
- [AdsConnect60]({{ site.baseurl }}/pt/funcoes/ads-connect-60/)
- [AdsGetConnectionType]({{ site.baseurl }}/pt/funcoes/ads-get-connection-type/)

---

[AdsGetConnectionType →]({{ site.baseurl }}/pt/funcoes/ads-get-connection-type/)
