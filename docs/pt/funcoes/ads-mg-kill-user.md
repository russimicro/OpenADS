---
title: AdsMgKillUser
layout: default
parent: Referência da API
nav_order: 27
permalink: /pt/funcoes/ads-mg-kill-user/
---

# AdsMgKillUser

Desconecta um usuário do servidor.

## Sintaxe

```c
UNSIGNED32 AdsMgKillUser(ADSHANDLE hMg, UNSIGNED8* pucUser,
                         UNSIGNED16 usOption);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hMg` | `ADSHANDLE` | Handle da conexão de gerenciamento. |
| `pucUser` | `UNSIGNED8*` | Nome do usuário a ser desconectado. |
| `usOption` | `UNSIGNED16` | Opções (0 = padrão). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsMgKillUser` força a desconexão de um usuário do servidor Advantage. O usuário é desconectado imediatamente e todas as suas conexões e bloqueios são liberados.

## Exemplo

```c
AdsMgKillUser(hMgmt, "usuario_problema", 0);
```

## Ver Também

- [AdsMgGetUserNames]({{ site.baseurl }}/pt/funcoes/ads-mg-get-user-names/)
- [AdsMgDisconnect]({{ site.baseurl }}/pt/funcoes/ads-mg-disconnect/)
- [AdsMgGetActivityInfo]({{ site.baseurl }}/pt/funcoes/ads-mg-get-activity-info/)

---

[AdsMgResetCommStats →]({{ site.baseurl }}/pt/funcoes/ads-mg-reset-comm-stats/)
