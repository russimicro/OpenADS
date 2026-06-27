---
title: AdsReleaseSavepoint
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-release-savepoint/
---

# AdsReleaseSavepoint

Liberta um savepoint.

## Sintaxe

```c
UNSIGNED32 AdsReleaseSavepoint(ADSHANDLE hConnect, UNSIGNED8* pucName);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `pucName` | `UNSIGNED8*` | Nome do savepoint. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INVALID_CONNECTION_HANDLE` se a conexão for inválida.

## Descrição

`AdsReleaseSavepoint` liberta um savepoint previamente criado. As alterações feitas desde o savepoint permanecem na transação.

## Exemplo

```c
AdsReleaseSavepoint(hConnect, "sp1");
```

## Ver Também

- [AdsCreateSavepoint]({{ site.baseurl }}/pt/funcoes/ads-create-savepoint/)
- [AdsRollbackTransaction80]({{ site.baseurl }}/pt/funcoes/ads-rollback-transaction-80/)
- [AdsBeginTransaction]({{ site.baseurl }}/pt/funcoes/ads-begin-transaction/)

---

[AdsRollbackTransaction80 →]({{ site.baseurl }}/pt/funcoes/ads-rollback-transaction-80/)
