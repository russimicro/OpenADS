---
title: AdsDDSetRefIntegrityProperty
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-dd-set-ref-integrity-property/
---

# AdsDDSetRefIntegrityProperty

Define uma propriedade de uma regra de integridade referencial.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDSetRefIntegrityProperty(ADSHANDLE hConnect, UNSIGNED8* pucName, UNSIGNED16 usPropertyID, void* pvProperty, UNSIGNED16 usPropertyLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão com o dicionário de dados. |
| `pucName` | `UNSIGNED8*` | Nome da regra de integridade referencial. |
| `usPropertyID` | `UNSIGNED16` | ID da propriedade a ser definida. |
| `pvProperty` | `void*` | Valor da propriedade. |
| `usPropertyLen` | `UNSIGNED16` | Comprimento do valor. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDSetRefIntegrityProperty` define uma propriedade de uma regra de integridade referencial (RI) no dicionário de dados. As propriedades que podem ser definidas incluem regras de atualização e exclusão.

## Exemplo

```c
AdsDDSetRefIntegrityProperty(hConnect, "RIClientes", ADS_DD_RI_UPDATE_RULE, &usCascade, sizeof(UNSIGNED16));
```

## Ver Também

- [AdsDDGetRefIntegrityProperty]({{ site.baseurl }}/pt/funcoes/ads-dd-get-ref-integrity-property/)
- [AdsDDCreateRefIntegrity]({{ site.baseurl }}/pt/funcoes/ads-dd-create-ref-integrity/)

---

[AdsDDSetTableProperty →]({{ site.baseurl }}/pt/funcoes/ads-dd-set-table-property/)
