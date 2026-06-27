---
title: AdsDDSetUserTableRights
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-dd-set-user-table-rights/
---

# AdsDDSetUserTableRights

Define os direitos de acesso de um usuário a uma tabela.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDSetUserTableRights(ADSHANDLE hConnect, UNSIGNED8* pucTableName, UNSIGNED8* pucUser, UNSIGNED32 ulRights);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão com o dicionário de dados. |
| `pucTableName` | `UNSIGNED8*` | Nome da tabela. |
| `pucUser` | `UNSIGNED8*` | Nome do usuário. |
| `ulRights` | `UNSIGNED32` | Nível de permissão a ser concedido. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDSetUserTableRights` define o nível de permissão que um usuário específico tem em relação a uma tabela. Os níveis de permissão são: NENHUM (0), LEITURA (1), ESCRITA (2), EXCLUSÃO (3) e COMPLETO (4).

## Exemplo

```c
AdsDDSetUserTableRights(hConnect, "Clientes", "usuario1", ADS_DD_TABLE_PERMISSION_READ);
```

## Ver Também

- [AdsDDGetUserTableRights]({{ site.baseurl }}/pt/funcoes/ads-dd-get-user-table-rights/)
- [AdsDDCreateUser]({{ site.baseurl }}/pt/funcoes/ads-dd-create-user/)

---

[AdsDDSetViewProperty →]({{ site.baseurl }}/pt/funcoes/ads-dd-set-view-property/)
