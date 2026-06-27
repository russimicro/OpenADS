---
title: AdsDDCreateUser
layout: default
parent: Referência da API
nav_order: 39
permalink: /pt/funcoes/ads-dd-create-user/
---

# AdsDDCreateUser

Cria um utilizador no dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDCreateUser(ADSHANDLE   hConnect,
                                      UNSIGNED8*  pucGroup,
                                      UNSIGNED8*  pucUser,
                                      UNSIGNED8*  pucPassword,
                                      UNSIGNED8*  pucDescription);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `pucGroup` | `UNSIGNED8*` | Nome do grupo (pode ser NULL). |
| `pucUser` | `UNSIGNED8*` | Nome do utilizador. |
| `pucPassword` | `UNSIGNED8*` | Palavra-passe. |
| `pucDescription` | `UNSIGNED8*` | Descrição (pode ser NULL). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDCreateUser` cria um novo utilizador no dicionário de dados, opcionalmente adicionando-o a um grupo.

## Exemplo

```c
AdsDDCreateUser(hConnect, "administradores", "admin", "senha123",
                "Administrador do sistema");
```

## Ver Também

- [AdsDDDeleteUser]({{ site.baseurl }}/pt/funcoes/ads-dd-delete-user/)
- [AdsDDAddUserToGroup]({{ site.baseurl }}/pt/funcoes/ads-dd-add-user-to-group/)

---

[AdsDDCreateView →]({{ site.baseurl }}/pt/funcoes/ads-dd-create-view/)
