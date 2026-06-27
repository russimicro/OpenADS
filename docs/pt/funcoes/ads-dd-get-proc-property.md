---
title: AdsDDGetProcProperty
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-dd-get-proc-property/
---

# AdsDDGetProcProperty

Obtém uma propriedade de um procedimento no dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDGetProcProperty(ADSHANDLE hConnect, UNSIGNED8* pucName, UNSIGNED16 usPropertyID, void* pvProperty, UNSIGNED16* pusPropertyLen);
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

`AdsDDGetProcProperty` recupera uma propriedade de um procedimento armazenado no dicionário de dados. Esta é uma variante da API que trabalha com procedimentos definidos no dicionário.

## Exemplo

```c
UNSIGNED16 usLen = 256;
UNSIGNED8 aucValue[256];

AdsDDGetProcProperty(hConnect, "MeuProc", ADS_DD_PROC_INPUT, aucValue, &usLen);
```

## Ver Também

- [AdsDDSetProcProperty]({{ site.baseurl }}/pt/funcoes/ads-dd-set-proc-property/)
- [AdsDDGetProcedureProperty]({{ site.baseurl }}/pt/funcoes/ads-dd-get-procedure-property/)

---

[AdsDDGetRefIntegrityProperty →]({{ site.baseurl }}/pt/funcoes/ads-dd-get-ref-integrity-property/)
