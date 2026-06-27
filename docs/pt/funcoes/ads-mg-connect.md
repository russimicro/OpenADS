---
title: AdsMgConnect
layout: default
parent: Referência da API
nav_order: 12
permalink: /pt/funcoes/ads-mg-connect/
---

# AdsMgConnect

Estabelece uma conexão de gerenciamento com um servidor Advantage.

## Sintaxe

```c
UNSIGNED32 AdsMgConnect(UNSIGNED8* pucServer, UNSIGNED8* pucUser,
                        UNSIGNED8* pucPwd, ADSHANDLE* phMg);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `pucServer` | `UNSIGNED8*` | Nome ou endereço do servidor. |
| `pucUser` | `UNSIGNED8*` | Nome do usuário. |
| `pucPwd` | `UNSIGNED8*` | Senha do usuário. |
| `phMg` | `ADSHANDLE*` | Ponteiro para handle de saída da conexão de gerenciamento. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsMgConnect` estabelece uma conexão dedicada para operações de gerenciamento com um servidor Advantage. Essa conexão é usada para funções de monitoramento e administração do servidor.

## Exemplo

```c
ADSHANDLE hMgmt;
AdsMgConnect("meuservidor", "admin", "senha", &hMgmt);
```

## Ver Também

- [AdsMgDisconnect]({{ site.baseurl }}/pt/funcoes/ads-mg-disconnect/)
- [AdsMgGetActivityInfo]({{ site.baseurl }}/pt/funcoes/ads-mg-get-activity-info/)
- [AdsMgGetServerType]({{ site.baseurl }}/pt/funcoes/ads-mg-get-server-type/)

---

[AdsMgDisconnect →]({{ site.baseurl }}/pt/funcoes/ads-mg-disconnect/)
