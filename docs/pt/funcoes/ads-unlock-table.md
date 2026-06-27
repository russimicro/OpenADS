---
title: AdsUnlockTable
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-unlock-table/
---

# AdsUnlockTable

Desbloqueia a tabela inteira.

## Sintaxe

```c
UNSIGNED32 AdsUnlockTable(ADSHANDLE hTable);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsUnlockTable` desbloqueia a tabela inteira previamente bloqueada com `AdsLockTable`.

Para tabelas remotas, o desbloqueio é gerido pelo servidor.

## Exemplo

```c
AdsUnlockTable(hTable);
```

## Ver Também

- [AdsLockTable]({{ site.baseurl }}/pt/funcoes/ads-lock-table/)
- [AdsIsTableLocked]({{ site.baseurl }}/pt/funcoes/ads-is-table-locked/)
- [AdsUnlockRecord]({{ site.baseurl }}/pt/funcoes/ads-unlock-record/)

---

[AdsPackTable →]({{ site.baseurl }}/pt/funcoes/ads-pack-table/)
