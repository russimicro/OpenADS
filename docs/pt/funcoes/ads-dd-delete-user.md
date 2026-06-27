---
title: AdsDDDeleteUser
layout: default
parent: Referência da API
nav_order: 41
permalink: /pt/funcoes/ads-dd-delete-user/
---

# AdsDDDeleteUser

Exclui um utilizador do dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDDeleteUser(ADSHANDLE  hConnect,
                                      UNSIGNED8* pucUser);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `pucUser` | `UNSIGNED8*` | Nome do utilizador. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDDeleteUser` exclui um utilizador do dicionário de dados, removendo-o de todos os grupos.

## Exemplo

```c
AdsDDDeleteUser(hConnect, "admin");
```

## Ver Também

- [AdsDDCreateUser]({{ site.baseurl }}/pt/funcoes/ads-dd-create-user/)
- [AdsDDAddUserToGroup]({{ site.baseurl }}/pt/funcoes/ads-dd-add-user-to-group/)

---

[AdsDDDropFunction →]({{ site.baseurl }}/pt/funcoes/ads-dd-drop-function/)
