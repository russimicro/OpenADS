---
title: AdsDDGetUserTableRights
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-dd-get-user-table-rights/
---

# AdsDDGetUserTableRights

Obtém os direitos de acesso de um usuário a uma tabela.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDGetUserTableRights(ADSHANDLE hConnect, UNSIGNED8* pucTableName, UNSIGNED8* pucUser, UNSIGNED32* pulRights);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão com o dicionário de dados. |
| `pucTableName` | `UNSIGNED8*` | Nome da tabela. |
| `pucUser` | `UNSIGNED8*` | Nome do usuário. |
| `pulRights` | `UNSIGNED32*` | Ponteiro para receber o nível de permissão. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDGetUserTableRights` recupera o nível de permissão que um usuário específico tem em relação a uma tabela. Os níveis de permissão são: NENHUM (0), LEITURA (1), ESCRITA (2), EXCLUSÃO (3) e COMPLETO (4).

## Exemplo

```c
UNSIGNED32 ulRights;

AdsDDGetUserTableRights(hConnect, "Clientes", "usuario1", &ulRights);
if (ulRights >= ADS_DD_TABLE_PERMISSION_READ) {
    // Usuário tem permissão de leitura
}
```

## Ver Também

- [AdsDDSetUserTableRights]({{ site.baseurl }}/pt/funcoes/ads-dd-set-user-table-rights/)
- [AdsDDCreateUser]({{ site.baseurl }}/pt/funcoes/ads-dd-create-user/)

---

[AdsDDGetViewProperty →]({{ site.baseurl }}/pt/funcoes/ads-dd-get-view-property/)
