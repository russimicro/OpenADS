---
title: AdsDDAddUserToGroup
layout: default
parent: Referência da API
nav_order: 31
permalink: /pt/funcoes/ads-dd-add-user-to-group/
---

# AdsDDAddUserToGroup

Adiciona um utilizador a um grupo.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDAddUserToGroup(ADSHANDLE hConnect,
                                          UNSIGNED8* pucGroup,
                                          UNSIGNED8* pucUser);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `pucGroup` | `UNSIGNED8*` | Nome do grupo. |
| `pucUser` | `UNSIGNED8*` | Nome do utilizador. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDAddUserToGroup` adiciona um utilizador existente a um grupo no dicionário de dados.

## Exemplo

```c
AdsDDAddUserToGroup(hConnect, "administradores", "admin");
```

## Ver Também

- [AdsDDRemoveUserFromGroup]({{ site.baseurl }}/pt/funcoes/ads-dd-remove-user-from-group/)
- [AdsDDCreateUser]({{ site.baseurl }}/pt/funcoes/ads-dd-create-user/)

---

[AdsDDCreate →]({{ site.baseurl }}/pt/funcoes/ads-dd-create/)
