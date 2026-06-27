---
title: AdsDDGetTriggerProperty
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-dd-get-trigger-property/
---

# AdsDDGetTriggerProperty

Obtém uma propriedade de um gatilho no dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDGetTriggerProperty(ADSHANDLE hConnect, UNSIGNED8* pucName, UNSIGNED16 usPropertyID, void* pvProperty, UNSIGNED16* pusPropertyLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão com o dicionário de dados. |
| `pucName` | `UNSIGNED8*` | Nome do gatilho. |
| `usPropertyID` | `UNSIGNED16` | ID da propriedade a ser obtida. |
| `pvProperty` | `void*` | Buffer para receber o valor da propriedade. |
| `pusPropertyLen` | `UNSIGNED16*` | Comprimento do buffer; retorna o comprimento do valor. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDGetTriggerProperty` recupera uma propriedade de um gatilho (trigger) no dicionário de dados. As propriedades incluem tabela, evento, container, nome do procedimento, habilitado, prioridade e comentários.

## Exemplo

```c
UNSIGNED16 usLen = 256;
UNSIGNED8 aucValue[256];

AdsDDGetTriggerProperty(hConnect, "TrgInsertClientes", ADS_DD_TRIGGER_TABLE, aucValue, &usLen);
```

## Ver Também

- [AdsDDSetTriggerProperty]({{ site.baseurl }}/pt/funcoes/ads-dd-set-trigger-property/)
- [AdsDDCreateTrigger]({{ site.baseurl }}/pt/funcoes/ads-dd-create-trigger/)

---

[AdsDDGetUserProperty →]({{ site.baseurl }}/pt/funcoes/ads-dd-get-user-property/)
