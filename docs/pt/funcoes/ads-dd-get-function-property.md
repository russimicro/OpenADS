---
title: AdsDDGetFunctionProperty
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-dd-get-function-property/
---

# AdsDDGetFunctionProperty

Obtém uma propriedade de uma função no dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDGetFunctionProperty(ADSHANDLE hConnect, UNSIGNED8* pucName, UNSIGNED16 usPropertyID, void* pvProperty, UNSIGNED16* pusPropertyLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão com o dicionário de dados. |
| `pucName` | `UNSIGNED8*` | Nome da função. |
| `usPropertyID` | `UNSIGNED16` | ID da propriedade a ser obtida. |
| `pvProperty` | `void*` | Buffer para receber o valor da propriedade. |
| `pusPropertyLen` | `UNSIGNED16*` | Comprimento do buffer; retorna o comprimento do valor. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDGetFunctionProperty` recupera uma propriedade de uma função definida pelo usuário no dicionário de dados. As propriedades incluem o container DLL e a função de implementação.

## Exemplo

```c
UNSIGNED16 usLen = 256;
UNSIGNED8 aucValue[256];

AdsDDGetFunctionProperty(hConnect, "MinhaFuncao", ADS_DD_PROC_CONTAINER, aucValue, &usLen);
```

## Ver Também

- [AdsDDSetFunctionProperty]({{ site.baseurl }}/pt/funcoes/ads-dd-set-function-property/)
- [AdsDDCreateFunction]({{ site.baseurl }}/pt/funcoes/ads-dd-create-function/)

---

[AdsDDGetIndexProperty →]({{ site.baseurl }}/pt/funcoes/ads-dd-get-index-property/)
