---
title: AdsDDSetProcProperty
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-dd-set-proc-property/
---

# AdsDDSetProcProperty

Define uma propriedade de um procedimento no dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDSetProcProperty(ADSHANDLE hConnect, UNSIGNED8* pucName, UNSIGNED16 usPropertyID, void* pvProperty, UNSIGNED16 usPropertyLen);
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

`AdsDDSetProcProperty` define uma propriedade de um procedimento armazenado no dicionário de dados. Esta é uma variante da API que trabalha com procedimentos definidos no dicionário.

## Exemplo

```c
AdsDDSetProcProperty(hConnect, "MeuProc", ADS_DD_PROC_INPUT, "@param1 N, @param2 C", 20);
```

## Ver Também

- [AdsDDGetProcProperty]({{ site.baseurl }}/pt/funcoes/ads-dd-get-proc-property/)
- [AdsDDGetProcedureProperty]({{ site.baseurl }}/pt/funcoes/ads-dd-get-procedure-property/)

---

[AdsDDSetRefIntegrityProperty →]({{ site.baseurl }}/pt/funcoes/ads-dd-set-ref-integrity-property/)
