---
title: AdsDDRemoveUserFromGroup
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-dd-remove-user-from-group/
---

# AdsDDRemoveUserFromGroup

Remove um usuário de um grupo no dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDRemoveUserFromGroup(ADSHANDLE hConnect, UNSIGNED8* pucGroup, UNSIGNED8* pucUser);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão com o dicionário de dados. |
| `pucGroup` | `UNSIGNED8*` | Nome do grupo. |
| `pucUser` | `UNSIGNED8*` | Nome do usuário a ser removido. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDRemoveUserFromGroup` remove a associação de um usuário a um grupo específico no dicionário de dados. O usuário permanece no dicionário, mas não faz mais parte do grupo especificado.

## Exemplo

```c
AdsDDRemoveUserFromGroup(hConnect, "Administradores", "usuario1");
```

## Ver Também

- [AdsDDAddUserToGroup]({{ site.baseurl }}/pt/funcoes/ads-dd-add-user-to-group/)
- [AdsDDCreateUser]({{ site.baseurl }}/pt/funcoes/ads-dd-create-user/)

---

[AdsDDSetDatabaseProperty →]({{ site.baseurl }}/pt/funcoes/ads-dd-set-database-property/)
