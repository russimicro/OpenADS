---
title: AdsDDGetViewProperty
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-dd-get-view-property/
---

# AdsDDGetViewProperty

Obtém uma propriedade de uma visão no dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDGetViewProperty(ADSHANDLE hConnect, UNSIGNED8* pucName, UNSIGNED16 usPropertyID, void* pvProperty, UNSIGNED16* pusPropertyLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão com o dicionário de dados. |
| `pucName` | `UNSIGNED8*` | Nome da visão. |
| `usPropertyID` | `UNSIGNED16` | ID da propriedade a ser obtida. |
| `pvProperty` | `void*` | Buffer para receber o valor da propriedade. |
| `pusPropertyLen` | `UNSIGNED16*` | Comprimento do buffer; retorna o comprimento do valor. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDGetViewProperty` recupera uma propriedade de uma visão (view) no dicionário de dados. As propriedades incluem a declaração SQL e comentários.

## Exemplo

```c
UNSIGNED16 usLen = 256;
UNSIGNED8 aucValue[256];

AdsDDGetViewProperty(hConnect, "VisaoClientesAtivos", ADS_DD_VIEW_STMT, aucValue, &usLen);
```

## Ver Também

- [AdsDDSetViewProperty]({{ site.baseurl }}/pt/funcoes/ads-dd-set-view-property/)
- [AdsDDCreateView]({{ site.baseurl }}/pt/funcoes/ads-dd-create-view/)

---

[AdsDDModifyLink →]({{ site.baseurl }}/pt/funcoes/ads-dd-modify-link/)
