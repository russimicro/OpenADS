---
title: AdsDDSetFunctionProperty
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-dd-set-function-property/
---

# AdsDDSetFunctionProperty

Define uma propriedade de uma função no dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDSetFunctionProperty(ADSHANDLE hConnect, UNSIGNED8* pucName, UNSIGNED16 usPropertyID, void* pvProperty, UNSIGNED16 usPropertyLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão com o dicionário de dados. |
| `pucName` | `UNSIGNED8*` | Nome da função. |
| `usPropertyID` | `UNSIGNED16` | ID da propriedade a ser definida. |
| `pvProperty` | `void*` | Valor da propriedade. |
| `usPropertyLen` | `UNSIGNED16` | Comprimento do valor. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDSetFunctionProperty` define uma propriedade de uma função definida pelo usuário no dicionário de dados. As propriedades que podem ser definidas incluem o container DLL e a função de implementação.

## Exemplo

```c
AdsDDSetFunctionProperty(hConnect, "MinhaFuncao", ADS_DD_PROC_CONTAINER, "minhadll.dll", 11);
```

## Ver Também

- [AdsDDGetFunctionProperty]({{ site.baseurl }}/pt/funcoes/ads-dd-get-function-property/)
- [AdsDDCreateFunction]({{ site.baseurl }}/pt/funcoes/ads-dd-create-function/)

---

[AdsDDSetIndexProperty →]({{ site.baseurl }}/pt/funcoes/ads-dd-set-index-property/)
