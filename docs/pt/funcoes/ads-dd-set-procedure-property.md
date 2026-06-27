---
title: AdsDDSetProcedureProperty
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-dd-set-procedure-property/
---

# AdsDDSetProcedureProperty

Define uma propriedade de um procedimento armazenado no dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDSetProcedureProperty(ADSHANDLE hConnect, UNSIGNED8* pucName, UNSIGNED16 usPropertyID, void* pvProperty, UNSIGNED16 usPropertyLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão com o dicionário de dados. |
| `pucName` | `UNSIGNED8*` | Nome do procedimento. |
| `usPropertyID` | `UNSIGNED16` | ID da propriedade a ser definida. |
| `pvProperty` | `void*` | Valor da propriedade. |
| `usPropertyLen` | `UNSIGNED16` | Comprimento do valor. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDSetProcedureProperty` define uma propriedade de um procedimento armazenado (stored procedure) no dicionário de dados. As propriedades que podem ser definidas incluem parâmetros de entrada/saída, container e nome do procedimento.

## Exemplo

```c
AdsDDSetProcedureProperty(hConnect, "MeuProc", ADS_DD_PROC_CONTAINER, "minhadll.dll", 11);
```

## Ver Também

- [AdsDDGetProcedureProperty]({{ site.baseurl }}/pt/funcoes/ads-dd-get-procedure-property/)
- [AdsDDCreateProcedure]({{ site.baseurl }}/pt/funcoes/ads-dd-create-procedure/)

---

[AdsDDSetProcProperty →]({{ site.baseurl }}/pt/funcoes/ads-dd-set-proc-property/)
