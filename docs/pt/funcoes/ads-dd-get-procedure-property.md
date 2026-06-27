---
title: AdsDDGetProcedureProperty
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-dd-get-procedure-property/
---

# AdsDDGetProcedureProperty

Obtém uma propriedade de um procedimento armazenado no dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDGetProcedureProperty(ADSHANDLE hConnect, UNSIGNED8* pucName, UNSIGNED16 usPropertyID, void* pvProperty, UNSIGNED16* pusPropertyLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão com o dicionário de dados. |
| `pucName` | `UNSIGNED8*` | Nome do procedimento. |
| `usPropertyID` | `UNSIGNED16` | ID da propriedade a ser obtida. |
| `pvProperty` | `void*` | Buffer para receber o valor da propriedade. |
| `pusPropertyLen` | `UNSIGNED16*` | Comprimento do buffer; retorna o comprimento do valor. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDGetProcedureProperty` recupera uma propriedade de um procedimento armazenado (stored procedure) no dicionário de dados. As propriedades incluem parâmetros de entrada/saída, container e nome do procedimento.

## Exemplo

```c
UNSIGNED16 usLen = 256;
UNSIGNED8 aucValue[256];

AdsDDGetProcedureProperty(hConnect, "MeuProc", ADS_DD_PROC_CONTAINER, aucValue, &usLen);
```

## Ver Também

- [AdsDDSetProcedureProperty]({{ site.baseurl }}/pt/funcoes/ads-dd-set-procedure-property/)
- [AdsDDCreateProcedure]({{ site.baseurl }}/pt/funcoes/ads-dd-create-procedure/)

---

[AdsDDGetProcProperty →]({{ site.baseurl }}/pt/funcoes/ads-dd-get-proc-property/)
