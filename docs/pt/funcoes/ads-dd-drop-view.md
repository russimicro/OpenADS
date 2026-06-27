---
title: AdsDDDropView
layout: default
parent: Referência da API
nav_order: 46
permalink: /pt/funcoes/ads-dd-drop-view/
---

# AdsDDDropView

Exclui uma visão (view) do dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDDropView(ADSHANDLE  hConnect,
                                    UNSIGNED8* pucName);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `pucName` | `UNSIGNED8*` | Nome da visão. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDDropView` exclui uma visão SQL do dicionário de dados.

## Exemplo

```c
AdsDDDropView(hConnect, "vw_clientes");
```

## Ver Também

- [AdsDDCreateView]({{ site.baseurl }}/pt/funcoes/ads-dd-create-view/)

---

[AdsDDFindClose →]({{ site.baseurl }}/pt/funcoes/ads-dd-find-close/)
