---
title: AdsDDSetIndexProperty
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-dd-set-index-property/
---

# AdsDDSetIndexProperty

Define uma propriedade de um índice no dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDSetIndexProperty(ADSHANDLE hConnect, UNSIGNED8* pucTableName, UNSIGNED8* pucTagName, UNSIGNED16 usPropertyID, void* pvProperty, UNSIGNED16 usPropertyLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão com o dicionário de dados. |
| `pucTableName` | `UNSIGNED8*` | Nome da tabela. |
| `pucTagName` | `UNSIGNED8*` | Nome da tag do índice. |
| `usPropertyID` | `UNSIGNED16` | ID da propriedade a ser definida. |
| `pvProperty` | `void*` | Valor da propriedade. |
| `usPropertyLen` | `UNSIGNED16` | Comprimento do valor. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDSetIndexProperty` define uma propriedade de um índice ou tag dentro de uma tabela no dicionário de dados. As propriedades que podem ser definidas incluem expressão, único, descendente e condição.

## Exemplo

```c
AdsDDSetIndexProperty(hConnect, "Clientes", "IdxNome", ADS_DD_INDEX_DESCENDING, &bTrue, sizeof(UNSIGNED16));
```

## Ver Também

- [AdsDDGetIndexProperty]({{ site.baseurl }}/pt/funcoes/ads-dd-get-index-property/)
- [AdsDDAddIndexFile]({{ site.baseurl }}/pt/funcoes/ads-dd-add-index-file/)

---

[AdsDDSetProcedureProperty →]({{ site.baseurl }}/pt/funcoes/ads-dd-set-procedure-property/)
