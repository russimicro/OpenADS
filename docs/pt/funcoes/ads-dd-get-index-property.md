---
title: AdsDDGetIndexProperty
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-dd-get-index-property/
---

# AdsDDGetIndexProperty

Obtém uma propriedade de um índice no dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDGetIndexProperty(ADSHANDLE hConnect, UNSIGNED8* pucTableName, UNSIGNED8* pucTagName, UNSIGNED16 usPropertyID, void* pvProperty, UNSIGNED16* pusPropertyLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão com o dicionário de dados. |
| `pucTableName` | `UNSIGNED8*` | Nome da tabela. |
| `pucTagName` | `UNSIGNED8*` | Nome da tag do índice. |
| `usPropertyID` | `UNSIGNED16` | ID da propriedade a ser obtida. |
| `pvProperty` | `void*` | Buffer para receber o valor da propriedade. |
| `pusPropertyLen` | `UNSIGNED16*` | Comprimento do buffer; retorna o comprimento do valor. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDGetIndexProperty` recupera uma propriedade de um índice ou tag dentro de uma tabela no dicionário de dados. As propriedades incluem nome do arquivo, expressão, único, descendente, condição e comprimento da chave.

## Exemplo

```c
UNSIGNED16 usLen = 256;
UNSIGNED8 aucValue[256];

AdsDDGetIndexProperty(hConnect, "Clientes", "IdxNome", ADS_DD_INDEX_EXPR, aucValue, &usLen);
```

## Ver Também

- [AdsDDSetIndexProperty]({{ site.baseurl }}/pt/funcoes/ads-dd-set-index-property/)
- [AdsDDAddIndexFile]({{ site.baseurl }}/pt/funcoes/ads-dd-add-index-file/)

---

[AdsDDGetProcedureProperty →]({{ site.baseurl }}/pt/funcoes/ads-dd-get-procedure-property/)
