---
title: AdsDDSetUserProperty
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-dd-set-user-property/
---

# AdsDDSetUserProperty

Define uma propriedade de um usuário no dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDSetUserProperty(ADSHANDLE hConnect, UNSIGNED8* pucUser, UNSIGNED16 usPropertyID, void* pvProperty, UNSIGNED16 usPropertyLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão com o dicionário de dados. |
| `pucUser` | `UNSIGNED8*` | Nome do usuário. |
| `usPropertyID` | `UNSIGNED16` | ID da propriedade a ser definida. |
| `pvProperty` | `void*` | Valor da propriedade. |
| `usPropertyLen` | `UNSIGNED16` | Comprimento do valor. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDSetUserProperty` define uma propriedade de um usuário no dicionário de dados. A propriedade mais comum a ser definida é a senha do usuário.

## Exemplo

```c
AdsDDSetUserProperty(hConnect, "admin", ADS_DD_USER_PASSWORD, "novasenha", 9);
```

## Ver Também

- [AdsDDGetUserProperty]({{ site.baseurl }}/pt/funcoes/ads-dd-get-user-property/)
- [AdsDDCreateUser]({{ site.baseurl }}/pt/funcoes/ads-dd-create-user/)

---

[AdsDDSetUserTableRights →]({{ site.baseurl }}/pt/funcoes/ads-dd-set-user-table-rights/)
