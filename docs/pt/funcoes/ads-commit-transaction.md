---
title: AdsCommitTransaction
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-commit-transaction/
---

# AdsCommitTransaction

Confirma uma transação.

## Sintaxe

```c
UNSIGNED32 AdsCommitTransaction(ADSHANDLE hConnect);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INVALID_CONNECTION_HANDLE` se a conexão for inválida.

## Descrição

`AdsCommitTransaction` confirma todas as operações feitas desde `AdsBeginTransaction`, persistindo as alterações em disco.

## Exemplo

```c
AdsBeginTransaction(hConnect);
// Operações de dados...
AdsCommitTransaction(hConnect);
```

## Ver Também

- [AdsBeginTransaction]({{ site.baseurl }}/pt/funcoes/ads-begin-transaction/)
- [AdsRollbackTransaction]({{ site.baseurl }}/pt/funcoes/ads-rollback-transaction/)
- [AdsInTransaction]({{ site.baseurl }}/pt/funcoes/ads-in-transaction/)

---

[AdsRollbackTransaction →]({{ site.baseurl }}/pt/funcoes/ads-rollback-transaction/)
