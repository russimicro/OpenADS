---
title: AdsBeginTransaction
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-begin-transaction/
---

# AdsBeginTransaction

Inicia uma transação.

## Sintaxe

```c
UNSIGNED32 AdsBeginTransaction(ADSHANDLE hConnect);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INVALID_CONNECTION_HANDLE` se a conexão for inválida.

## Descrição

`AdsBeginTransaction` inicia uma nova transação. Todas as operações subsequentes são agrupadas até `AdsCommitTransaction` ou `AdsRollbackTransaction`.

## Exemplo

```c
AdsBeginTransaction(hConnect);
// Operações de dados...
AdsCommitTransaction(hConnect);  // ou AdsRollbackTransaction
```

## Ver Também

- [AdsCommitTransaction]({{ site.baseurl }}/pt/funcoes/ads-commit-transaction/)
- [AdsRollbackTransaction]({{ site.baseurl }}/pt/funcoes/ads-rollback-transaction/)
- [AdsInTransaction]({{ site.baseurl }}/pt/funcoes/ads-in-transaction/)

---

[AdsCommitTransaction →]({{ site.baseurl }}/pt/funcoes/ads-commit-transaction/)
