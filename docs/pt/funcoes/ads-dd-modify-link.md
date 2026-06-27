---
title: AdsDDModifyLink
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-dd-modify-link/
---

# AdsDDModifyLink

Modifica as propriedades de um link no dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDModifyLink(ADSHANDLE hConnect, UNSIGNED8* pucAlias, UNSIGNED8* pucPath, UNSIGNED8* pucUser, UNSIGNED8* pucPassword, UNSIGNED16 usOptions);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão com o dicionário de dados. |
| `pucAlias` | `UNSIGNED8*` | Alias do link a ser modificado. |
| `pucPath` | `UNSIGNED8*` | Novo caminho do link. |
| `pucUser` | `UNSIGNED8*` | Novo nome de usuário. |
| `pucPassword` | `UNSIGNED8*` | Nova senha. |
| `usOptions` | `UNSIGNED16` | Opções de modificação. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDModifyLink` modifica as propriedades de um link existente no dicionário de dados. Um link permite acessar tabelas em outro servidor ou localização de rede.

## Exemplo

```c
AdsDDModifyLink(hConnect, "LinkRemoto", "\\\\servidor\\dados\\tabela.dbf", "user", "senha", 0);
```

## Ver Também

- [AdsDDCreateLink]({{ site.baseurl }}/pt/funcoes/ads-dd-create-link/)
- [AdsDDDropLink]({{ site.baseurl }}/pt/funcoes/ads-dd-drop-link/)

---

[AdsDDRemoveIndexFile →]({{ site.baseurl }}/pt/funcoes/ads-dd-remove-index-file/)
