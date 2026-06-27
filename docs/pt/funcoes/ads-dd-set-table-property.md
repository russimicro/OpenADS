---
title: AdsDDSetTableProperty
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-dd-set-table-property/
---

# AdsDDSetTableProperty

Define uma propriedade de uma tabela no dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDSetTableProperty(ADSHANDLE hConnect, UNSIGNED8* pucTableName, UNSIGNED16 usPropertyID, void* pvProperty, UNSIGNED16 usPropertyLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão com o dicionário de dados. |
| `pucTableName` | `UNSIGNED8*` | Nome da tabela. |
| `usPropertyID` | `UNSIGNED16` | ID da propriedade a ser definida. |
| `pvProperty` | `void*` | Valor da propriedade. |
| `usPropertyLen` | `UNSIGNED16` | Comprimento do valor. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDSetTableProperty` define uma propriedade de uma tabela no dicionário de dados. As propriedades que podem ser definidas incluem expressão de validação, mensagem de validação, automação de criação e configurações de criptografia.

## Exemplo

```c
AdsDDSetTableProperty(hConnect, "Clientes", ADS_DD_TABLE_AUTO_CREATE, &bTrue, sizeof(UNSIGNED16));
```

## Ver Também

- [AdsDDGetTableProperty]({{ site.baseurl }}/pt/funcoes/ads-dd-get-table-property/)
- [AdsDDAddTable]({{ site.baseurl }}/pt/funcoes/ads-dd-add-table/)

---

[AdsDDSetTriggerProperty →]({{ site.baseurl }}/pt/funcoes/ads-dd-set-trigger-property/)
