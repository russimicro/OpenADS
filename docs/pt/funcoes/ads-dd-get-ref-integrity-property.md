---
title: AdsDDGetRefIntegrityProperty
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-dd-get-ref-integrity-property/
---

# AdsDDGetRefIntegrityProperty

Obtém uma propriedade de uma regra de integridade referencial.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDGetRefIntegrityProperty(ADSHANDLE hConnect, UNSIGNED8* pucName, UNSIGNED16 usPropertyID, void* pvProperty, UNSIGNED16* pusPropertyLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão com o dicionário de dados. |
| `pucName` | `UNSIGNED8*` | Nome da regra de integridade referencial. |
| `usPropertyID` | `UNSIGNED16` | ID da propriedade a ser obtida. |
| `pvProperty` | `void*` | Buffer para receber o valor da propriedade. |
| `pusPropertyLen` | `UNSIGNED16*` | Comprimento do buffer; retorna o comprimento do valor. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDGetRefIntegrityProperty` recupera uma propriedade de uma regra de integridade referencial (RI) no dicionário de dados. As propriedades incluem tabela pai, tabela filha, tags, e regras de atualização/exclusão.

## Exemplo

```c
UNSIGNED16 usLen = 256;
UNSIGNED8 aucValue[256];

AdsDDGetRefIntegrityProperty(hConnect, "RIClientes", ADS_DD_RI_PARENT, aucValue, &usLen);
```

## Ver Também

- [AdsDDSetRefIntegrityProperty]({{ site.baseurl }}/pt/funcoes/ads-dd-set-ref-integrity-property/)
- [AdsDDCreateRefIntegrity]({{ site.baseurl }}/pt/funcoes/ads-dd-create-ref-integrity/)

---

[AdsDDGetTableProperty →]({{ site.baseurl }}/pt/funcoes/ads-dd-get-table-property/)
