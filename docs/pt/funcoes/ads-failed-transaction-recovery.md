---
title: AdsFailedTransactionRecovery
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-failed-transaction-recovery/
---

# AdsFailedTransactionRecovery

Executa a recuperação de transações falhas.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsFailedTransactionRecovery(UNSIGNED8* pucServer);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `pucServer` | `UNSIGNED8*` | Nome ou endereço do servidor. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsFailedTransactionRecovery` inicia o processo de recuperação de transações que falharam no servidor. Esta função é usada para recuperar dados after uma falha de energia ou outro problema que interrompeu uma transação em andamento.

## Exemplo

```c
AdsFailedTransactionRecovery("localhost");
```

## Ver Também

- [AdsBeginTransaction]({{ site.baseurl }}/pt/funcoes/ads-begin-transaction/)
- [AdsCommitTransaction]({{ site.baseurl }}/pt/funcoes/ads-commit-transaction/)
- [AdsRollbackTransaction]({{ site.baseurl }}/pt/funcoes/ads-rollback-transaction/)

---

[AdsFetchWhere →]({{ site.baseurl }}/pt/funcoes/ads-fetch-where/)
