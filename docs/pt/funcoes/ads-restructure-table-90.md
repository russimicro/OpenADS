---
title: AdsRestructureTable90
layout: default
parent: Referência da API
nav_order: 35
permalink: /pt/funcoes/ads-restructure-table-90/
---

# AdsRestructureTable90

Modifica a estrutura de uma tabela com suporte a colação.

## Sintaxe

```c
UNSIGNED32 AdsRestructureTable90(ADSHANDLE hConnect,
                                 UNSIGNED8* pucTableName,
                                 UNSIGNED8* pucPassword,
                                 UNSIGNED16 usTableType,
                                 UNSIGNED16 usCharType,
                                 UNSIGNED16 usLockType,
                                 UNSIGNED16 usCheckRights,
                                 UNSIGNED8* pucAddFields,
                                 UNSIGNED8* pucDeleteFields,
                                 UNSIGNED8* pucChangeFields,
                                 UNSIGNED8* pucCollation);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `pucTableName` | `UNSIGNED8*` | Nome da tabela a ser modificada. |
| `pucPassword` | `UNSIGNED8*` | Senha da tabela (ou NULL). |
| `usTableType` | `UNSIGNED16` | Novo tipo da tabela (0 = manter). |
| `usCharType` | `UNSIGNED16` | Novo tipo de caracteres (0 = manter). |
| `usLockType` | `UNSIGNED16` | Novo tipo de bloqueio (0 = manter). |
| `usCheckRights` | `UNSIGNED16` | Verificar direitos (0 = manter). |
| `pucAddFields` | `UNSIGNED8*` | Lista de campos a adicionar (ou NULL). |
| `pucDeleteFields` | `UNSIGNED8*` | Lista de campos a remover (ou NULL). |
| `pucChangeFields` | `UNSIGNED8*` | Lista de campos a modificar (ou NULL). |
| `pucCollation` | `UNSIGNED8*` | Nome da colação (NULL para manter). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsRestructureTable90` modifica a estrutura de uma tabela existente, permitindo adicionar, remover ou alterar campos. Essa variante (versão 90) também permite especificar a colação da tabela.

## Exemplo

```c
AdsRestructureTable90(hConnect, "Clientes", NULL, 0, 0, 0, 0,
                       "Email C(100)", "Telefone", NULL, "LATIN1");
```

## Ver Também

- [AdsRestructureTable]({{ site.baseurl }}/pt/funcoes/ads-restructure-table/)
- [AdsCreateTable]({{ site.baseurl }}/pt/funcoes/ads-create-table/)
- [AdsOpenTable90]({{ site.baseurl }}/pt/funcoes/ads-open-table-90/)

---

[AdsSetBinary →]({{ site.baseurl }}/pt/funcoes/ads-set-binary/)
