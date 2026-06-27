---
title: AdsDDGetUserProperty
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-dd-get-user-property/
---

# AdsDDGetUserProperty

Obtém uma propriedade de um usuário no dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDGetUserProperty(ADSHANDLE hConnect, UNSIGNED8* pucUser, UNSIGNED16 usPropertyID, void* pvProperty, UNSIGNED16* pusPropertyLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão com o dicionário de dados. |
| `pucUser` | `UNSIGNED8*` | Nome do usuário. |
| `usPropertyID` | `UNSIGNED16` | ID da propriedade a ser obtida. |
| `pvProperty` | `void*` | Buffer para receber o valor da propriedade. |
| `pusPropertyLen` | `UNSIGNED16*` | Comprimento do buffer; retorna o comprimento do valor. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDGetUserProperty` recupera uma propriedade de um usuário no dicionário de dados. As propriedades incluem senha, associação a grupos e tentativas de login incorretas.

## Exemplo

```c
UNSIGNED16 usLen = 256;
UNSIGNED8 aucValue[256];

AdsDDGetUserProperty(hConnect, "admin", ADS_DD_USER_GROUP_MEMBERSHIP, aucValue, &usLen);
```

## Ver Também

- [AdsDDSetUserProperty]({{ site.baseurl }}/pt/funcoes/ads-dd-set-user-property/)
- [AdsDDCreateUser]({{ site.baseurl }}/pt/funcoes/ads-dd-create-user/)

---

[AdsDDGetUserTableRights →]({{ site.baseurl }}/pt/funcoes/ads-dd-get-user-table-rights/)
