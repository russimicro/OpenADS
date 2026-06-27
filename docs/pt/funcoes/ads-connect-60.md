---
title: AdsConnect60
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-connect-60/
---

# AdsConnect60

Estabelece uma conexão com o servidor (versão 6.0+).

## Sintaxe

```c
UNSIGNED32 AdsConnect60(UNSIGNED8* pucServer, UNSIGNED16 usServerType,
                        UNSIGNED8* pucUser, UNSIGNED8* pucPwd,
                        UNSIGNED32 ulOptions, ADSHANDLE* phConnect);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `pucServer` | `UNSIGNED8*` | Caminho do diretório ou URI (tcp:// ou tls://). |
| `usServerType` | `UNSIGNED16` | Tipo do servidor (reservado). |
| `pucUser` | `UNSIGNED8*` | Nome do utilizador (para autenticação). |
| `pucPwd` | `UNSIGNED8*` | Palavra-passe (para autenticação). |
| `ulOptions` | `UNSIGNED32` | Opções (reservadas). |
| `phConnect` | `ADSHANDLE*` | Ponteiro para receber o handle da conexão. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se a conexão falhar.

## Descrição

`AdsConnect60` é a versão estendida de `AdsConnect`, suportando autenticação e protocolos seguros (TLS).

URI suportados:
- `/caminho` - Conexão local
- `tcp://host:port/dir` - Conexão remota
- `tls://host:port/dir` - Conexão TLS remota

## Exemplo

```c
ADSHANDLE hConnect;
AdsConnect60("tcp://192.168.1.100:16262//dados", ADS_REMOTE_SERVER,
             "user", "pass", 0, &hConnect);
```

## Ver Também

- [AdsConnect]({{ site.baseurl }}/pt/funcoes/ads-connect/)
- [AdsDisconnect]({{ site.baseurl }}/pt/funcoes/ads-disconnect/)
- [AdsOpenTable]({{ site.baseurl }}/pt/funcoes/ads-open-table/)

---

[AdsDisconnect →]({{ site.baseurl }}/pt/funcoes/ads-disconnect/)
