---
title: AdsDDCreateRefIntegrity
layout: default
parent: Referência da API
nav_order: 36
permalink: /pt/funcoes/ads-dd-create-ref-integrity/
---

# AdsDDCreateRefIntegrity

Cria uma restrição de integridade referencial.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDCreateRefIntegrity(ADSHANDLE   hConnect,
                                              UNSIGNED8*  pucName,
                                              UNSIGNED8*  pucFailTable,
                                              UNSIGNED8*  pucParent,
                                              UNSIGNED8*  pucParentTag,
                                              UNSIGNED8*  pucChild,
                                              UNSIGNED8*  pucChildTag,
                                              UNSIGNED16  usUpdateOption,
                                              UNSIGNED16  usDeleteOption);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `pucName` | `UNSIGNED8*` | Nome da restrição RI. |
| `pucFailTable` | `UNSIGNED8*` | Tabela de falha. |
| `pucParent` | `UNSIGNED8*` | Tabela pai. |
| `pucParentTag` | `UNSIGNED8*` | Tag da tabela pai. |
| `pucChild` | `UNSIGNED8*` | Tabela filho. |
| `pucChildTag` | `UNSIGNED8*` | Tag da tabela filho. |
| `usUpdateOption` | `UNSIGNED16` | Opção de atualização (ADS_DD_RI_CASCADE, etc.). |
| `usDeleteOption` | `UNSIGNED16` | Opção de exclusão (ADS_DD_RI_CASCADE, etc.). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDCreateRefIntegrity` cria uma restrição de integridade referencial entre tabelas pai e filho.

## Exemplo

```c
AdsDDCreateRefIntegrity(hConnect, "ri_pedidos", "fail.dbf",
                        "clientes", "cod_cliente",
                        "pedidos", "cod_cliente",
                        ADS_DD_RI_CASCADE, ADS_DD_RI_RESTRICT);
```

## Ver Também

- [AdsDDCreateRefIntegrity62]({{ site.baseurl }}/pt/funcoes/ads-dd-create-ref-integrity-62/)
- [AdsDDRemoveRefIntegrity]({{ site.baseurl }}/pt/funcoes/ads-dd-remove-ref-integrity/)

---

[AdsDDCreateRefIntegrity62 →]({{ site.baseurl }}/pt/funcoes/ads-dd-create-ref-integrity-62/)
