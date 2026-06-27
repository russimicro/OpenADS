---
title: AdsDeleteIndex
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-delete-index/
---

# AdsDeleteIndex

Exclui um índice aberto.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDeleteIndex(ADSHANDLE hIndex);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hIndex` | `ADSHANDLE` | Handle do índice a ser excluído. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDeleteIndex` fecha e exclui um índice aberto. O arquivo físico do índice não é excluído do disco; apenas o handle é liberado. Para excluir o arquivo físico, use AdsDeleteFile após fechar o índice.

## Exemplo

```c
AdsDeleteIndex(hIndex);
```

## Ver Também

- [AdsCloseIndex]({{ site.baseurl }}/pt/funcoes/ads-close-index/)
- [AdsDeleteFile]({{ site.baseurl }}/pt/funcoes/ads-delete-file/)

---

[AdsFailedTransactionRecovery →]({{ site.baseurl }}/pt/funcoes/ads-failed-transaction-recovery/)
