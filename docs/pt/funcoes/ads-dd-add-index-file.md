---
title: AdsDDAddIndexFile
layout: default
parent: Referência da API
nav_order: 27
permalink: /pt/funcoes/ads-dd-add-index-file/
---

# AdsDDAddIndexFile

Adiciona um arquivo de índice ao dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDAddIndexFile(ADSHANDLE hConnect,
                                        UNSIGNED8* pucTableName,
                                        UNSIGNED8* pucIndexFile,
                                        UNSIGNED8* pucComment);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão do dicionário de dados. |
| `pucTableName` | `UNSIGNED8*` | Nome da tabela. |
| `pucIndexFile` | `UNSIGNED8*` | Nome do arquivo de índice. |
| `pucComment` | `UNSIGNED8*` | Comentário (pode ser NULL). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDAddIndexFile` adiciona um arquivo de índice existente ao dicionário de dados para uma tabela específica.

## Exemplo

```c
AdsDDAddIndexFile(hConnect, "clientes", "clientes.adx", NULL);
```

## Ver Também

- [AdsDDRemoveIndexFile]({{ site.baseurl }}/pt/funcoes/ads-dd-remove-index-file/)
- [AdsDDAddTable]({{ site.baseurl }}/pt/funcoes/ads-dd-add-table/)

---

[AdsDDAddProcedure →]({{ site.baseurl }}/pt/funcoes/ads-dd-add-procedure/)
