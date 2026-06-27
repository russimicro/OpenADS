---
title: AdsDDCreateTrigger
layout: default
parent: Referência da API
nav_order: 38
permalink: /pt/funcoes/ads-dd-create-trigger/
---

# AdsDDCreateTrigger

Cria um gatilho (trigger) no dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDCreateTrigger(ADSHANDLE   hConnect,
                                         UNSIGNED8*  pucName,
                                         UNSIGNED8*  pucTable,
                                         UNSIGNED32  ulType,
                                         UNSIGNED32  ulOptions,
                                         UNSIGNED8*  pucContainer,
                                         UNSIGNED8*  pucProcedure,
                                         UNSIGNED32  ulPriority);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `pucName` | `UNSIGNED8*` | Nome do gatilho. |
| `pucTable` | `UNSIGNED8*` | Tabela associada. |
| `ulType` | `UNSIGNED32` | Tipo do evento (ADS_BEFORE_INSERT, etc.). |
| `ulOptions` | `UNSIGNED32` | Opções. |
| `pucContainer` | `UNSIGNED8*` | Container (DLL/arquivo). |
| `pucProcedure` | `UNSIGNED8*` | Nome da procedimento. |
| `ulPriority` | `UNSIGNED32` | Prioridade do gatilho. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDCreateTrigger` cria um gatilho que é executado automaticamente quando ocorrem eventos de inserção, atualização ou exclusão na tabela.

Tipos de evento: `ADS_BEFORE_INSERT`, `ADS_AFTER_INSERT`, `ADS_BEFORE_UPDATE`, `ADS_AFTER_UPDATE`, `ADS_BEFORE_DELETE`, `ADS_AFTER_DELETE`.

## Exemplo

```c
AdsDDCreateTrigger(hConnect, "trg_cliente", "clientes",
                   ADS_AFTER_INSERT, 0, "meudll.dll",
                   "MeuTrigger", 1);
```

## Ver Também

- [AdsDDDropTrigger]({{ site.baseurl }}/pt/funcoes/ads-dd-drop-trigger/)

---

[AdsDDCreateUser →]({{ site.baseurl }}/pt/funcoes/ads-dd-create-user/)
