---
title: AdsDDCreateFunction
layout: default
parent: Referência da API
nav_order: 33
permalink: /pt/funcoes/ads-dd-create-function/
---

# AdsDDCreateFunction

Cria uma função no dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDCreateFunction(ADSHANDLE   hConnect,
                                          UNSIGNED8*  pucName,
                                          UNSIGNED8*  pucContainer,
                                          UNSIGNED8*  pucImplementation,
                                          UNSIGNED8*  pucRetType,
                                          UNSIGNED8*  pucInParams,
                                          UNSIGNED8*  pucComment);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `pucName` | `UNSIGNED8*` | Nome da função. |
| `pucContainer` | `UNSIGNED8*` | Container (DLL/arquivo). |
| `pucImplementation` | `UNSIGNED8*` | Nome da implementação. |
| `pucRetType` | `UNSIGNED8*` | Tipo de retorno. |
| `pucInParams` | `UNSIGNED8*` | Parâmetros de entrada. |
| `pucComment` | `UNSIGNED8*` | Comentário. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDCreateFunction` cria uma função definida pelo utilizador no dicionário de dados.

## Exemplo

```c
AdsDDCreateFunction(hConnect, "minha_func", "meudll.dll",
                    "MinhaFuncao", "N(10)", "C(50)", NULL);
```

## Ver Também

- [AdsDDDropFunction]({{ site.baseurl }}/pt/funcoes/ads-dd-drop-function/)

---

[AdsDDCreateLink →]({{ site.baseurl }}/pt/funcoes/ads-dd-create-link/)
