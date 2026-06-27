---
title: AdsDDGetTableProperty
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-dd-get-table-property/
---

# AdsDDGetTableProperty

Obtém uma propriedade de uma tabela no dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDGetTableProperty(ADSHANDLE hConnect, UNSIGNED8* pucTableName, UNSIGNED16 usPropertyID, void* pvProperty, UNSIGNED16* pusPropertyLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão com o dicionário de dados. |
| `pucTableName` | `UNSIGNED8*` | Nome da tabela. |
| `usPropertyID` | `UNSIGNED16` | ID da propriedade a ser obtida. |
| `pvProperty` | `void*` | Buffer para receber o valor da propriedade. |
| `pusPropertyLen` | `UNSIGNED16*` | Comprimento do buffer; retorna o comprimento do valor. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDGetTableProperty` recupera uma propriedade de uma tabela no dicionário de dados. As propriedades incluem expressão de validação, mensagem de validação, chave primária, tipo, caminho, contagem de campos e configurações de criptografia.

## Exemplo

```c
UNSIGNED16 usLen = 256;
UNSIGNED8 aucValue[256];

AdsDDGetTableProperty(hConnect, "Clientes", ADS_DD_TABLE_PATH, aucValue, &usLen);
```

## Ver Também

- [AdsDDSetTableProperty]({{ site.baseurl }}/pt/funcoes/ads-dd-set-table-property/)
- [AdsDDAddTable]({{ site.baseurl }}/pt/funcoes/ads-dd-add-table/)

---

[AdsDDGetTriggerProperty →]({{ site.baseurl }}/pt/funcoes/ads-dd-get-trigger-property/)
