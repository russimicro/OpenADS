---
title: AdsDDRemoveTrigger
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-dd-remove-trigger/
---

# AdsDDRemoveTrigger

Remove um gatilho do dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDRemoveTrigger(ADSHANDLE hConnect, UNSIGNED8* pucName);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão com o dicionário de dados. |
| `pucName` | `UNSIGNED8*` | Nome do gatilho a ser removido. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDRemoveTrigger` remove um gatilho (trigger) existente do dicionário de dados. Após a remoção, o gatilho não será mais executado quando os eventos de inserção, atualização ou exclusão ocorrerem na tabela associada.

## Exemplo

```c
AdsDDRemoveTrigger(hConnect, "TrgInsertClientes");
```

## Ver Também

- [AdsDDCreateTrigger]({{ site.baseurl }}/pt/funcoes/ads-dd-create-trigger/)
- [AdsDDGetTriggerProperty]({{ site.baseurl }}/pt/funcoes/ads-dd-get-trigger-property/)

---

[AdsDDRemoveUserFromGroup →]({{ site.baseurl }}/pt/funcoes/ads-dd-remove-user-from-group/)
