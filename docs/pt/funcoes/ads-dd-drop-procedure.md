---
title: AdsDDDropProcedure
layout: default
parent: Referência da API
nav_order: 44
permalink: /pt/funcoes/ads-dd-drop-procedure/
---

# AdsDDDropProcedure

Exclui um procedimento do dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDDropProcedure(ADSHANDLE  hConnect,
                                         UNSIGNED8* pucName);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `pucName` | `UNSIGNED8*` | Nome do procedimento. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDDropProcedure` exclui um procedimento armazenado do dicionário de dados.

## Exemplo

```c
AdsDDDropProcedure(hConnect, "meu_proc");
```

## Ver Também

- [AdsDDCreateProcedure]({{ site.baseurl }}/pt/funcoes/ads-dd-create-procedure/)
- [AdsDDAddProcedure]({{ site.baseurl }}/pt/funcoes/ads-dd-add-procedure/)

---

[AdsDDDropTrigger →]({{ site.baseurl }}/pt/funcoes/ads-dd-drop-trigger/)
