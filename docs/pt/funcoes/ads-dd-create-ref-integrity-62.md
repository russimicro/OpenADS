---
title: AdsDDCreateRefIntegrity62
layout: default
parent: Referência da API
nav_order: 37
permalink: /pt/funcoes/ads-dd-create-ref-integrity-62/
---

# AdsDDCreateRefIntegrity62

Cria uma restrição de integridade referencial (versão 6.2+).

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDCreateRefIntegrity62(ADSHANDLE   hConnect,
                                                UNSIGNED8*  pucName,
                                                UNSIGNED8*  pucFail,
                                                UNSIGNED8*  pucParent,
                                                UNSIGNED8*  pucParentTag,
                                                UNSIGNED8*  pucChild,
                                                UNSIGNED8*  pucChildTag,
                                                UNSIGNED16  usUpdate,
                                                UNSIGNED16  usDelete,
                                                UNSIGNED8*  pucNoPrimaryError,
                                                UNSIGNED8*  pucCascadeError);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `pucName` | `UNSIGNED8*` | Nome da restrição RI. |
| `pucFail` | `UNSIGNED8*` | Tabela de falha. |
| `pucParent` | `UNSIGNED8*` | Tabela pai. |
| `pucParentTag` | `UNSIGNED8*` | Tag da tabela pai. |
| `pucChild` | `UNSIGNED8*` | Tabela filho. |
| `pucChildTag` | `UNSIGNED8*` | Tag da tabela filho. |
| `usUpdate` | `UNSIGNED16` | Opção de atualização. |
| `usDelete` | `UNSIGNED16` | Opção de exclusão. |
| `pucNoPrimaryError` | `UNSIGNED8*` | Mensagem de erro de chave primária. |
| `pucCascadeError` | `UNSIGNED8*` | Mensagem de erro de cascata. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDCreateRefIntegrity62` é a versão estendida de `AdsDDCreateRefIntegrity`, adicionando mensagens de erro personalizadas.

## Exemplo

```c
AdsDDCreateRefIntegrity62(hConnect, "ri_pedidos", "fail.dbf",
                          "clientes", "cod_cliente",
                          "pedidos", "cod_cliente",
                          ADS_DD_RI_CASCADE, ADS_DD_RI_RESTRICT,
                          "Chave primária não encontrada",
                          "Erro de cascata");
```

## Ver Também

- [AdsDDCreateRefIntegrity]({{ site.baseurl }}/pt/funcoes/ads-dd-create-ref-integrity/)
- [AdsDDRemoveRefIntegrity]({{ site.baseurl }}/pt/funcoes/ads-dd-remove-ref-integrity/)

---

[AdsDDCreateTrigger →]({{ site.baseurl }}/pt/funcoes/ads-dd-create-trigger/)
