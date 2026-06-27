---
title: AdsDDCreateView
layout: default
parent: Referência da API
nav_order: 40
permalink: /pt/funcoes/ads-dd-create-view/
---

# AdsDDCreateView

Cria uma visão (view) no dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDCreateView(ADSHANDLE   hConnect,
                                      UNSIGNED8*  pucName,
                                      UNSIGNED8*  pucComment,
                                      UNSIGNED8*  pucSQL);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `pucName` | `UNSIGNED8*` | Nome da visão. |
| `pucComment` | `UNSIGNED8*` | Comentário (pode ser NULL). |
| `pucSQL` | `UNSIGNED8*` | Instrução SQL da visão. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDCreateView` cria uma visão SQL no dicionário de dados.

## Exemplo

```c
AdsDDCreateView(hConnect, "vw_clientes", "Visão de clientes",
                "SELECT COD, NOME FROM clientes WHERE ATIVO = 1");
```

## Ver Também

- [AdsDDDropView]({{ site.baseurl }}/pt/funcoes/ads-dd-drop-view/)

---

[AdsDDDeleteUser →]({{ site.baseurl }}/pt/funcoes/ads-dd-delete-user/)
