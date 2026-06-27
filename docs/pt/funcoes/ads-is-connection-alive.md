---
title: AdsIsConnectionAlive
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-is-connection-alive/
---

# AdsIsConnectionAlive

Verifica se uma conexão ainda está ativa (ping de heartbeat).

## Sintaxe

```c
UNSIGNED32 AdsIsConnectionAlive(ADSHANDLE hConnect, UNSIGNED16 *pbAlive);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `pbAlive` | `UNSIGNED16*` | Saída — `ADS_TRUE` se a conexão estiver ativa, `ADS_FALSE` caso contrário. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso.

## Descrição

`AdsIsConnectionAlive` envia um ping de heartbeat ao servidor para
verificar se a conexão ainda está ativa. Para conexões locais,
retorna sempre `ADS_TRUE`. Para conexões remotas, realiza uma
viagem de ida e volta na rede para confirmar que o servidor é
alcançável.

## Exemplo

```c
ADSHANDLE hConn = 0;
AdsConnect60("tcp://server:6247", NULL, NULL, NULL, 0, &hConn);
unsigned short bAlive = 0;
AdsIsConnectionAlive(hConn, &bAlive);
if (bAlive == ADS_TRUE)
    printf("A conexão está ativa\n");
else
    printf("A conexão está inativa\n");
AdsDisconnect(hConn);
```

## Ver Também

- [AdsConnect60]({{ site.baseurl }}/pt/funcoes/ads-connect60/)
- [AdsGetConnectionType]({{ site.baseurl }}/pt/funcoes/ads-get-connection-type/)
- [AdsDisconnect]({{ site.baseurl }}/pt/funcoes/ads-disconnect/)

---

[← AdsIsFound]({{ site.baseurl }}/pt/funcoes/ads-is-found/)
[AdsIsNull →]({{ site.baseurl }}/pt/funcoes/ads-is-null/)
