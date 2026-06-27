---
title: AdsDDDropTrigger
layout: default
parent: Referência da API
nav_order: 45
permalink: /pt/funcoes/ads-dd-drop-trigger/
---

# AdsDDDropTrigger

Exclui um gatilho (trigger) do dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDDropTrigger(ADSHANDLE  hConnect,
                                       UNSIGNED8* pucName);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `pucName` | `UNSIGNED8*` | Nome do gatilho. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDDropTrigger` exclui um gatilho do dicionário de dados.

## Exemplo

```c
AdsDDDropTrigger(hConnect, "trg_cliente");
```

## Ver Também

- [AdsDDCreateTrigger]({{ site.baseurl }}/pt/funcoes/ads-dd-create-trigger/)

---

[AdsDDDropView →]({{ site.baseurl }}/pt/funcoes/ads-dd-drop-view/)
