---
title: AdsRollbackTransaction
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-rollback-transaction/
---

# AdsRollbackTransaction

Reverte uma transação.

## Sintaxe

```c
UNSIGNED32 AdsRollbackTransaction(ADSHANDLE hConnect);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INVALID_CONNECTION_HANDLE` se a conexão for inválida.

## Descrição

`AdsRollbackTransaction` reverte todas as operações feitas desde `AdsBeginTransaction`, descartando as alterações.

## Exemplo

```c
AdsBeginTransaction(hConnect);
// Operações de dados...
AdsRollbackTransaction(hConnect);  // Alterações descartadas
```

## Ver Também

- [AdsBeginTransaction]({{ site.baseurl }}/pt/funcoes/ads-begin-transaction/)
- [AdsCommitTransaction]({{ site.baseurl }}/pt/funcoes/ads-commit-transaction/)
- [AdsInTransaction]({{ site.baseurl }}/pt/funcoes/ads-in-transaction/)

---

[AdsInTransaction →]({{ site.baseurl }}/pt/funcoes/ads-in-transaction/)
