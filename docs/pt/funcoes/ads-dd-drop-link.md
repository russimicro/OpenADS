---
title: AdsDDDropLink
layout: default
parent: Referência da API
nav_order: 43
permalink: /pt/funcoes/ads-dd-drop-link/
---

# AdsDDDropLink

Exclui um link do dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDDropLink(ADSHANDLE   hConnect,
                                    UNSIGNED8*  pucAlias,
                                    UNSIGNED16  usOptions);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `pucAlias` | `UNSIGNED8*` | Alias do link. |
| `usOptions` | `UNSIGNED16` | Opções (reservado). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDDropLink` exclui um link do dicionário de dados.

## Exemplo

```c
AdsDDDropLink(hConnect, "remoto", 0);
```

## Ver Também

- [AdsDDCreateLink]({{ site.baseurl }}/pt/funcoes/ads-dd-create-link/)
- [AdsDDModifyLink]({{ site.baseurl }}/pt/funcoes/ads-dd-modify-link/)

---

[AdsDDDropProcedure →]({{ site.baseurl }}/pt/funcoes/ads-dd-drop-procedure/)
