---
title: AdsDDRemoveIndexFile
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-dd-remove-index-file/
---

# AdsDDRemoveIndexFile

Remove um arquivo de índice do dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDRemoveIndexFile(ADSHANDLE hConnect, UNSIGNED8* pucTableName, UNSIGNED8* pucIndexFile, UNSIGNED16 usDeleteFile);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão com o dicionário de dados. |
| `pucTableName` | `UNSIGNED8*` | Nome da tabela. |
| `pucIndexFile` | `UNSIGNED8*` | Nome do arquivo de índice. |
| `usDeleteFile` | `UNSIGNED16` | Se verdadeiro, exclui o arquivo de índice do disco. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDRemoveIndexFile` remove a referência de um arquivo de índice de uma tabela no dicionário de dados. Se `usDeleteFile` for verdadeiro, o arquivo físico também será excluído do disco.

## Exemplo

```c
AdsDDRemoveIndexFile(hConnect, "Clientes", "Clientes.cdx", 1);
```

## Ver Também

- [AdsDDAddIndexFile]({{ site.baseurl }}/pt/funcoes/ads-dd-add-index-file/)
- [AdsDeleteIndex]({{ site.baseurl }}/pt/funcoes/ads-delete-index/)

---

[AdsDDRemoveProcedure →]({{ site.baseurl }}/pt/funcoes/ads-dd-remove-procedure/)
