---
title: AdsDDSetFieldProperty
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-dd-set-field-property/
---

# AdsDDSetFieldProperty

Define uma propriedade de um campo no dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDSetFieldProperty(ADSHANDLE hConnect, UNSIGNED8* pucTableName, UNSIGNED8* pucFieldName, UNSIGNED16 usPropertyID, void* pvProperty, UNSIGNED16 usPropertyLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão com o dicionário de dados. |
| `pucTableName` | `UNSIGNED8*` | Nome da tabela. |
| `pucFieldName` | `UNSIGNED8*` | Nome do campo. |
| `usPropertyID` | `UNSIGNED16` | ID da propriedade a ser definida. |
| `pvProperty` | `void*` | Valor da propriedade. |
| `usPropertyLen` | `UNSIGNED16` | Comprimento do valor. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDSetFieldProperty` define uma propriedade de um campo dentro de uma tabela no dicionário de dados. As propriedades que podem ser definidas incluem obrigatório, padrão, regra de validação e comentários.

## Exemplo

```c
AdsDDSetFieldProperty(hConnect, "Clientes", "Email", ADS_DD_FIELD_REQUIRED, &bTrue, sizeof(UNSIGNED16));
```

## Ver Também

- [AdsDDGetFieldProperty]({{ site.baseurl }}/pt/funcoes/ads-dd-get-field-property/)
- [AdsDDGetTableProperty]({{ site.baseurl }}/pt/funcoes/ads-dd-get-table-property/)

---

[AdsDDSetFunctionProperty →]({{ site.baseurl }}/pt/funcoes/ads-dd-set-function-property/)
