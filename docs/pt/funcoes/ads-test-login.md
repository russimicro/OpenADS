---
title: AdsTestLogin
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-test-login/
---

# AdsTestLogin

Testa o login no servidor.

## Sintaxe

```c
UNSIGNED32 AdsTestLogin(UNSIGNED8* pucServer, UNSIGNED16 usServerType,
                        UNSIGNED8* pucUser, UNSIGNED8* pucPwd,
                        UNSIGNED32 ulOptions);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `pucServer` | `UNSIGNED8*` | Servidor. |
| `usServerType` | `UNSIGNED16` | Tipo do servidor. |
| `pucUser` | `UNSIGNED8*` | Nome do utilizador. |
| `pucPwd` | `UNSIGNED8*` | Palavra-passe. |
| `ulOptions` | `UNSIGNED32` | Opções. |

## Valor de Retorno

`AE_SUCCESS` (0) sempre.

## Descrição

`AdsTestLogin` testa as credenciais de login. No OpenADS, retorna sempre sucesso.

## Ver Também

- [AdsConnect]({{ site.baseurl }}/pt/funcoes/ads-connect/)
- [AdsConnect60]({{ site.baseurl }}/pt/funcoes/ads-connect-60/)
- [AdsDisconnect]({{ site.baseurl }}/pt/funcoes/ads-disconnect/)

---

[AdsTestRecLocks →]({{ site.baseurl }}/pt/funcoes/ads-test-rec-locks/)
