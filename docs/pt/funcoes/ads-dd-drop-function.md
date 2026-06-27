---
title: AdsDDDropFunction
layout: default
parent: Referência da API
nav_order: 42
permalink: /pt/funcoes/ads-dd-drop-function/
---

# AdsDDDropFunction

Exclui uma função do dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDDropFunction(ADSHANDLE  hConnect,
                                        UNSIGNED8* pucName);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `pucName` | `UNSIGNED8*` | Nome da função. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDDropFunction` exclui uma função definida pelo utilizador do dicionário de dados.

## Exemplo

```c
AdsDDDropFunction(hConnect, "minha_func");
```

## Ver Também

- [AdsDDCreateFunction]({{ site.baseurl }}/pt/funcoes/ads-dd-create-function/)

---

[AdsDDDropLink →]({{ site.baseurl }}/pt/funcoes/ads-dd-drop-link/)
