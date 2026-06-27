---
title: AdsDDAddProcedure
layout: default
parent: Referência da API
nav_order: 28
permalink: /pt/funcoes/ads-dd-add-procedure/
---

# AdsDDAddProcedure

Adiciona um procedimento ao dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDAddProcedure(ADSHANDLE   hConnect,
                                        UNSIGNED8*  pucName,
                                        UNSIGNED8*  pucContainer,
                                        UNSIGNED8*  pucProcName,
                                        UNSIGNED32  ulInvokeOption,
                                        UNSIGNED8*  pucInParams,
                                        UNSIGNED8*  pucOutParams,
                                        UNSIGNED8*  pucComments);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `pucName` | `UNSIGNED8*` | Nome do procedimento. |
| `pucContainer` | `UNSIGNED8*` | Container (DLL/arquivo). |
| `pucProcName` | `UNSIGNED8*` | Nome da função no container. |
| `ulInvokeOption` | `UNSIGNED32` | Opção de invocação. |
| `pucInParams` | `UNSIGNED8*` | Parâmetros de entrada. |
| `pucOutParams` | `UNSIGNED8*` | Parâmetros de saída. |
| `pucComments` | `UNSIGNED8*` | Comentários. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDAddProcedure` adiciona um procedimento armazenado ao dicionário de dados.

## Exemplo

```c
AdsDDAddProcedure(hConnect, "meu_proc", "meudll.dll",
                  "MinhaFuncao", 0, "C(50)", "N(10)", NULL);
```

## Ver Também

- [AdsDDDropProcedure]({{ site.baseurl }}/pt/funcoes/ads-dd-drop-procedure/)
- [AdsDDCreateProcedure]({{ site.baseurl }}/pt/funcoes/ads-dd-create-procedure/)

---

[AdsDDAddTable →]({{ site.baseurl }}/pt/funcoes/ads-dd-add-table/)
