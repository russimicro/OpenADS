---
title: AdsInTransaction
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-in-transaction/
---

# AdsInTransaction

Verifica se a conexão está numa transação.

## Sintaxe

```c
UNSIGNED32 AdsInTransaction(ADSHANDLE hConnect, UNSIGNED16* pbInTx);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `pbInTx` | `UNSIGNED16*` | Ponteiro para receber 1 se estiver em transação, 0 caso contrário. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INVALID_CONNECTION_HANDLE` se a conexão for inválida.

## Descrição

`AdsInTransaction` verifica se a conexão está atualmente numa transação ativa.

## Exemplo

```c
UNSIGNED16 pbInTx;
AdsInTransaction(hConnect, &pbInTx);
if (pbInTx) {
    // Está em transação
}
```

## Ver Também

- [AdsBeginTransaction]({{ site.baseurl }}/pt/funcoes/ads-begin-transaction/)
- [AdsCommitTransaction]({{ site.baseurl }}/pt/funcoes/ads-commit-transaction/)
- [AdsRollbackTransaction]({{ site.baseurl }}/pt/funcoes/ads-rollback-transaction/)

---

[AdsConnect →]({{ site.baseurl }}/pt/funcoes/ads-connect/)
