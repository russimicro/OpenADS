---
title: AdsRollbackTransaction80
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-rollback-transaction-80/
---

# AdsRollbackTransaction80

Reverte uma transação para um savepoint.

## Sintaxe

```c
UNSIGNED32 AdsRollbackTransaction80(ADSHANDLE hConnect, UNSIGNED8* pucSavepoint,
                                    UNSIGNED32 ulOptions);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `pucSavepoint` | `UNSIGNED8*` | Nome do savepoint (vazio para rollback completo). |
| `ulOptions` | `UNSIGNED32` | Opções (reservadas). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INVALID_CONNECTION_HANDLE` se a conexão for inválida.

## Descrição

`AdsRollbackTransaction80` reverte uma transação para um savepoint específico. Se o nome do savepoint for nulo, faz rollback completo da transação.

## Exemplo

```c
AdsRollbackTransaction80(hConnect, "sp1", 0);  // Rollback para savepoint
AdsRollbackTransaction80(hConnect, nullptr, 0);  // Rollback completo
```

## Ver Também

- [AdsCreateSavepoint]({{ site.baseurl }}/pt/funcoes/ads-create-savepoint/)
- [AdsReleaseSavepoint]({{ site.baseurl }}/pt/funcoes/ads-release-savepoint/)
- [AdsRollbackTransaction]({{ site.baseurl }}/pt/funcoes/ads-rollback-transaction/)

---

[AdsFindFirstTable →]({{ site.baseurl }}/pt/funcoes/ads-find-first-table/)
