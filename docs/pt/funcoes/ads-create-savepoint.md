---
title: AdsCreateSavepoint
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-create-savepoint/
---

# AdsCreateSavepoint

Cria um savepoint dentro de uma transação.

## Sintaxe

```c
UNSIGNED32 AdsCreateSavepoint(ADSHANDLE hConnect, UNSIGNED8* pucName,
                              UNSIGNED32 ulOptions);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `pucName` | `UNSIGNED8*` | Nome do savepoint. |
| `ulOptions` | `UNSIGNED32` | Opções (reservadas). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INVALID_CONNECTION_HANDLE` se a conexão for inválida.

## Descrição

`AdsCreateSavepoint` cria um savepoint dentro de uma transação ativa, permitindo rollback parcial.

## Exemplo

```c
AdsBeginTransaction(hConnect);
AdsCreateSavepoint(hConnect, "sp1", 0);
// Operações...
AdsReleaseSavepoint(hConnect, "sp1");
// ou AdsRollbackToSavepoint(hConnect, "sp1");
```

## Ver Também

- [AdsReleaseSavepoint]({{ site.baseurl }}/pt/funcoes/ads-release-savepoint/)
- [AdsBeginTransaction]({{ site.baseurl }}/pt/funcoes/ads-begin-transaction/)
- [AdsCommitTransaction]({{ site.baseurl }}/pt/funcoes/ads-commit-transaction/)

---

[AdsReleaseSavepoint →]({{ site.baseurl }}/pt/funcoes/ads-release-savepoint/)
