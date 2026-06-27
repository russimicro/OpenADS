---
title: AdsDDRemoveTable
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-dd-remove-table/
---

# AdsDDRemoveTable

Remove uma tabela do dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDRemoveTable(ADSHANDLE hConnect, UNSIGNED8* pucAlias, UNSIGNED16 usDeleteFiles);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão com o dicionário de dados. |
| `pucAlias` | `UNSIGNED8*` | Alias da tabela a ser removida. |
| `usDeleteFiles` | `UNSIGNED16` | Se verdadeiro, exclui os arquivos da tabela do disco. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDRemoveTable` remove uma tabela do dicionário de dados. Se `usDeleteFiles` for verdadeiro, os arquivos físicos da tabela (DBF, CDX, FPT) também serão excluídos do disco.

## Exemplo

```c
AdsDDRemoveTable(hConnect, "Clientes", 0);
```

## Ver Também

- [AdsDDAddTable]({{ site.baseurl }}/pt/funcoes/ads-dd-add-table/)
- [AdsDeleteFile]({{ site.baseurl }}/pt/funcoes/ads-delete-file/)

---

[AdsDDRemoveTrigger →]({{ site.baseurl }}/pt/funcoes/ads-dd-remove-trigger/)
