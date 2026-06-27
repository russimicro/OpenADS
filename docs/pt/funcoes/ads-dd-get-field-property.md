---
title: AdsDDGetFieldProperty
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-dd-get-field-property/
---

# AdsDDGetFieldProperty

Obtém uma propriedade de um campo no dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDGetFieldProperty(ADSHANDLE hConnect, UNSIGNED8* pucTableName, UNSIGNED8* pucFieldName, UNSIGNED16 usPropertyID, void* pvProperty, UNSIGNED16* pusPropertyLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão com o dicionário de dados. |
| `pucTableName` | `UNSIGNED8*` | Nome da tabela. |
| `pucFieldName` | `UNSIGNED8*` | Nome do campo. |
| `usPropertyID` | `UNSIGNED16` | ID da propriedade a ser obtida. |
| `pvProperty` | `void*` | Buffer para receber o valor da propriedade. |
| `pusPropertyLen` | `UNSIGNED16*` | Comprimento do buffer; retorna o comprimento do valor. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDGetFieldProperty` recupera uma propriedade específica de um campo dentro de uma tabela no dicionário de dados. As propriedades incluem nome, tipo, comprimento, decimais, obrigatório, padrão, regra de validação e comentários.

## Exemplo

```c
UNSIGNED16 usLen = 256;
UNSIGNED8 aucValue[256];

AdsDDGetFieldProperty(hConnect, "Clientes", "Nome", ADS_DD_FIELD_TYPE, aucValue, &usLen);
```

## Ver Também

- [AdsDDSetFieldProperty]({{ site.baseurl }}/pt/funcoes/ads-dd-set-field-property/)
- [AdsDDGetTableProperty]({{ site.baseurl }}/pt/funcoes/ads-dd-get-table-property/)

---

[AdsDDGetFunctionProperty →]({{ site.baseurl }}/pt/funcoes/ads-dd-get-function-property/)
