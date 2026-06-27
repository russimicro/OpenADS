---
title: AdsDDAddTable90
layout: default
parent: Referência da API
nav_order: 30
permalink: /pt/funcoes/ads-dd-add-table-90/
---

# AdsDDAddTable90

Adiciona uma tabela ao dicionário de dados (versão 9.0+).

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDAddTable90(ADSHANDLE  hConnect,
                                      UNSIGNED8* pucAlias,
                                      UNSIGNED8* pucTablePath,
                                      UNSIGNED16 usTableType,
                                      UNSIGNED16 usCharType,
                                      UNSIGNED8* pucIndexPath,
                                      UNSIGNED8* pucComment,
                                      UNSIGNED8* pucCollation);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `pucAlias` | `UNSIGNED8*` | Alias da tabela. |
| `pucTablePath` | `UNSIGNED8*` | Caminho do arquivo da tabela. |
| `usTableType` | `UNSIGNED16` | Tipo da tabela. |
| `usCharType` | `UNSIGNED16` | Tipo de caracteres. |
| `pucIndexPath` | `UNSIGNED8*` | Caminho do arquivo de índice. |
| `pucComment` | `UNSIGNED8*` | Comentário. |
| `pucCollation` | `UNSIGNED8*` | Collation para a tabela. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDAddTable90` é a versão estendida de `AdsDDAddTable`, adicionando suporte a collation.

## Exemplo

```c
AdsDDAddTable90(hConnect, "clientes", "C:\\dados\\clientes.adt",
                ADS_ADT, ADS_ANSI, NULL, "Tabela de clientes", NULL);
```

## Ver Também

- [AdsDDAddTable]({{ site.baseurl }}/pt/funcoes/ads-dd-add-table/)
- [AdsDDRemoveTable]({{ site.baseurl }}/pt/funcoes/ads-dd-remove-table/)

---

[AdsDDAddUserToGroup →]({{ site.baseurl }}/pt/funcoes/ads-dd-add-user-to-group/)
