---
title: AdsGetAllLocks
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-all-locks/
---

# AdsGetAllLocks

Retorna todos os registos bloqueados.

## Sintaxe

```c
UNSIGNED32 AdsGetAllLocks(ADSHANDLE hTable, UNSIGNED32* paRecnos,
                          UNSIGNED16* pusCount);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `paRecnos` | `UNSIGNED32*` | Array para receber os números dos registos. |
| `pusCount` | `UNSIGNED16*` | Ponteiro para o tamanho do array. Na saída, contém o número de bloqueios. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsGetAllLocks` retorna todos os números de registos bloqueados pela conexão atual.

## Exemplo

```c
UNSIGNED32 paRecnos[100];
UNSIGNED16 usCount = 100;
AdsGetAllLocks(hTable, paRecnos, &usCount);
// paRecnos contém os registos bloqueados
```

## Ver Também

- [AdsLockRecord]({{ site.baseurl }}/pt/funcoes/ads-lock-record/)
- [AdsUnlockRecord]({{ site.baseurl }}/pt/funcoes/ads-unlock-record/)
- [AdsIsRecordLocked]({{ site.baseurl }}/pt/funcoes/ads-is-record-locked/)

---

[AdsLockRecord →]({{ site.baseurl }}/pt/funcoes/ads-lock-record/)
