---
title: AdsLockTable
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-lock-table/
---

# AdsLockTable

Bloqueia a tabela inteira.

## Sintaxe

```c
UNSIGNED32 AdsLockTable(ADSHANDLE hTable);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsLockTable` bloqueia a tabela inteira para acesso exclusivo.

Para tabelas remotas, o bloqueio é gerido pelo servidor.

## Exemplo

```c
AdsLockTable(hTable);
// Apenas esta conexão pode aceder à tabela
AdsUnlockTable(hTable);
```

## Ver Também

- [AdsUnlockTable]({{ site.baseurl }}/pt/funcoes/ads-unlock-table/)
- [AdsIsTableLocked]({{ site.baseurl }}/pt/funcoes/ads-is-table-locked/)
- [AdsLockRecord]({{ site.baseurl }}/pt/funcoes/ads-lock-record/)

---

[AdsUnlockTable →]({{ site.baseurl }}/pt/funcoes/ads-unlock-table/)
