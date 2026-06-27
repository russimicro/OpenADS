---
title: AdsDDSetTriggerProperty
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-dd-set-trigger-property/
---

# AdsDDSetTriggerProperty

Define uma propriedade de um gatilho no dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDSetTriggerProperty(ADSHANDLE hConnect, UNSIGNED8* pucName, UNSIGNED16 usPropertyID, void* pvProperty, UNSIGNED16 usPropertyLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão com o dicionário de dados. |
| `pucName` | `UNSIGNED8*` | Nome do gatilho. |
| `usPropertyID` | `UNSIGNED16` | ID da propriedade a ser definida. |
| `pvProperty` | `void*` | Valor da propriedade. |
| `usPropertyLen` | `UNSIGNED16` | Comprimento do valor. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDSetTriggerProperty` define uma propriedade de um gatilho (trigger) no dicionário de dados. As propriedades que podem ser definidas incluem habilitado, prioridade e comentários.

## Exemplo

```c
AdsDDSetTriggerProperty(hConnect, "TrgInsertClientes", ADS_DD_TRIGGER_ENABLED, &bTrue, sizeof(UNSIGNED16));
```

## Ver Também

- [AdsDDGetTriggerProperty]({{ site.baseurl }}/pt/funcoes/ads-dd-get-trigger-property/)
- [AdsDDCreateTrigger]({{ site.baseurl }}/pt/funcoes/ads-dd-create-trigger/)

---

[AdsDDSetUserProperty →]({{ site.baseurl }}/pt/funcoes/ads-dd-set-user-property/)
