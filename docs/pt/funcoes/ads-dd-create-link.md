---
title: AdsDDCreateLink
layout: default
parent: Referência da API
nav_order: 34
permalink: /pt/funcoes/ads-dd-create-link/
---

# AdsDDCreateLink

Cria um link no dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDCreateLink(ADSHANDLE   hConnect,
                                      UNSIGNED8*  pucAlias,
                                      UNSIGNED8*  pucPath,
                                      UNSIGNED8*  pucUser,
                                      UNSIGNED8*  pucPassword,
                                      UNSIGNED16  usOptions);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `pucAlias` | `UNSIGNED8*` | Alias do link. |
| `pucPath` | `UNSIGNED8*` | Caminho do destino do link. |
| `pucUser` | `UNSIGNED8*` | Nome do utilizador. |
| `pucPassword` | `UNSIGNED8*` | Palavra-passe. |
| `usOptions` | `UNSIGNED16` | Opções (reservado). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDCreateLink` cria um link para uma tabela em outro dicionário de dados ou servidor.

## Exemplo

```c
AdsDDCreateLink(hConnect, "remoto", "tcp://192.168.1.100:16262//dados",
                "user", "pass", 0);
```

## Ver Também

- [AdsDDDropLink]({{ site.baseurl }}/pt/funcoes/ads-dd-drop-link/)
- [AdsDDModifyLink]({{ site.baseurl }}/pt/funcoes/ads-dd-modify-link/)

---

[AdsDDCreateProcedure →]({{ site.baseurl }}/pt/funcoes/ads-dd-create-procedure/)
