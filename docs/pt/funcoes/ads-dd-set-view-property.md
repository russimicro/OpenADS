---
title: AdsDDSetViewProperty
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-dd-set-view-property/
---

# AdsDDSetViewProperty

Define uma propriedade de uma visão no dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDSetViewProperty(ADSHANDLE hConnect, UNSIGNED8* pucName, UNSIGNED16 usPropertyID, void* pvProperty, UNSIGNED16 usPropertyLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão com o dicionário de dados. |
| `pucName` | `UNSIGNED8*` | Nome da visão. |
| `usPropertyID` | `UNSIGNED16` | ID da propriedade a ser definida. |
| `pvProperty` | `void*` | Valor da propriedade. |
| `usPropertyLen` | `UNSIGNED16` | Comprimento do valor. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDSetViewProperty` define uma propriedade de uma visão (view) no dicionário de dados. As propriedades que podem ser definidas incluem a declaração SQL e comentários.

## Exemplo

```c
AdsDDSetViewProperty(hConnect, "VisaoClientes", ADS_DD_VIEW_STMT, "SELECT * FROM Clientes WHERE Ativo = 1", 35);
```

## Ver Também

- [AdsDDGetViewProperty]({{ site.baseurl }}/pt/funcoes/ads-dd-get-view-property/)
- [AdsDDCreateView]({{ site.baseurl }}/pt/funcoes/ads-dd-create-view/)

---

[AdsDeleteCustomKey →]({{ site.baseurl }}/pt/funcoes/ads-delete-custom-key/)
