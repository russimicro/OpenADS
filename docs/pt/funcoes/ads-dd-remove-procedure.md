---
title: AdsDDRemoveProcedure
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-dd-remove-procedure/
---

# AdsDDRemoveProcedure

Remove um procedimento armazenado do dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDRemoveProcedure(ADSHANDLE hConnect, UNSIGNED8* pucName);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão com o dicionário de dados. |
| `pucName` | `UNSIGNED8*` | Nome do procedimento a ser removido. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDRemoveProcedure` remove um procedimento armazenado (stored procedure) do dicionário de dados. Esta é uma variante da API de remoção de procedimentos.

## Exemplo

```c
AdsDDRemoveProcedure(hConnect, "MeuProc");
```

## Ver Também

- [AdsDDDropProcedure]({{ site.baseurl }}/pt/funcoes/ads-dd-drop-procedure/)
- [AdsDDCreateProcedure]({{ site.baseurl }}/pt/funcoes/ads-dd-create-procedure/)

---

[AdsDDRemoveRefIntegrity →]({{ site.baseurl }}/pt/funcoes/ads-dd-remove-ref-integrity/)
