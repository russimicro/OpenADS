---
title: AdsDDSetDatabaseProperty
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-dd-set-database-property/
---

# AdsDDSetDatabaseProperty

Define uma propriedade do dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDSetDatabaseProperty(ADSHANDLE hConnect, UNSIGNED16 usPropertyID, void* pvProperty, UNSIGNED16 usPropertyLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão com o dicionário de dados. |
| `usPropertyID` | `UNSIGNED16` | ID da propriedade a ser definida. |
| `pvProperty` | `void*` | Valor da propriedade. |
| `usPropertyLen` | `UNSIGNED16` | Comprimento do valor. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDSetDatabaseProperty` define uma propriedade do dicionário de dados especificada pelo ID. As propriedades que podem ser definidas incluem comentários, senha de administrador, caminhos padrão e configurações de criptografia.

## Exemplo

```c
AdsDDSetDatabaseProperty(hConnect, ADS_DD_COMMENT, "Meu banco de dados", 17);
```

## Ver Também

- [AdsDDGetDatabaseProperty]({{ site.baseurl }}/pt/funcoes/ads-dd-get-database-property/)
- [AdsDDCreate]({{ site.baseurl }}/pt/funcoes/ads-dd-create/)

---

[AdsDDSetFieldProperty →]({{ site.baseurl }}/pt/funcoes/ads-dd-set-field-property/)
