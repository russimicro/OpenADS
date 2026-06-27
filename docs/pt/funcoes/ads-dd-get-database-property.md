---
title: AdsDDGetDatabaseProperty
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-dd-get-database-property/
---

# AdsDDGetDatabaseProperty

Obtém uma propriedade do dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDGetDatabaseProperty(ADSHANDLE hConnect, UNSIGNED16 usPropertyID, void* pvProperty, UNSIGNED16* pusPropertyLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão com o dicionário de dados. |
| `usPropertyID` | `UNSIGNED16` | ID da propriedade a ser obtida. |
| `pvProperty` | `void*` | Buffer para receber o valor da propriedade. |
| `pusPropertyLen` | `UNSIGNED16*` | Comprimento do buffer; retorna o comprimento do valor. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDGetDatabaseProperty` recupera uma propriedade do dicionário de dados especificada pelo ID. As propriedades disponíveis incluem comentários, senha de administrador, caminhos padrão e configurações de criptografia.

## Exemplo

```c
UNSIGNED16 usLen = 256;
UNSIGNED8 aucComment[256];

AdsDDGetDatabaseProperty(hConnect, ADS_DD_COMMENT, aucComment, &usLen);
```

## Ver Também

- [AdsDDSetDatabaseProperty]({{ site.baseurl }}/pt/funcoes/ads-dd-set-database-property/)
- [AdsDDCreate]({{ site.baseurl }}/pt/funcoes/ads-dd-create/)

---

[AdsDDGetFieldProperty →]({{ site.baseurl }}/pt/funcoes/ads-dd-get-field-property/)
