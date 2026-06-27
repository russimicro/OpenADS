---
title: AdsDDAddTable
layout: default
parent: Referência da API
nav_order: 29
permalink: /pt/funcoes/ads-dd-add-table/
---

# AdsDDAddTable

Adiciona uma tabela ao dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDAddTable(ADSHANDLE   hConnect,
                                    UNSIGNED8*  pucAlias,
                                    UNSIGNED8*  pucTablePath,
                                    UNSIGNED16  usFileType,
                                    UNSIGNED16  usCharType,
                                    UNSIGNED8*  pucIndexPath,
                                    UNSIGNED8*  pucComment);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `pucAlias` | `UNSIGNED8*` | Alias da tabela. |
| `pucTablePath` | `UNSIGNED8*` | Caminho do arquivo da tabela. |
| `usFileType` | `UNSIGNED16` | Tipo do arquivo (ADS_CDX, ADS_ADT, etc.). |
| `usCharType` | `UNSIGNED16` | Tipo de caracteres. |
| `pucIndexPath` | `UNSIGNED8*` | Caminho do arquivo de índice. |
| `pucComment` | `UNSIGNED8*` | Comentário. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDAddTable` adiciona uma tabela existente ao dicionário de dados.

## Exemplo

```c
AdsDDAddTable(hConnect, "clientes", "C:\\dados\\clientes.adt",
              ADS_ADT, ADS_ANSI, NULL, "Tabela de clientes");
```

## Ver Também

- [AdsDDAddTable90]({{ site.baseurl }}/pt/funcoes/ads-dd-add-table-90/)
- [AdsDDRemoveTable]({{ site.baseurl }}/pt/funcoes/ads-dd-remove-table/)

---

[AdsDDAddTable90 →]({{ site.baseurl }}/pt/funcoes/ads-dd-add-table-90/)
