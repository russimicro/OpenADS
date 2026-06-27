---
title: AdsIsRecordLocked
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-is-record-locked/
---

# AdsIsRecordLocked

Verifica se um registo está bloqueado.

## Sintaxe

```c
UNSIGNED32 AdsIsRecordLocked(ADSHANDLE hTable, UNSIGNED32 ulRecord,
                             UNSIGNED16* pbLocked);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `ulRecord` | `UNSIGNED32` | Número do registo (0 para o atual). |
| `pbLocked` | `UNSIGNED16*` | Ponteiro para receber 1 se estiver bloqueado, 0 caso contrário. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsIsRecordLocked` verifica se um registo está bloqueado pela conexão atual.

Para tabelas remotas, a função retorna 0 (os bloqueios não são introspectados).

## Exemplo

```c
UNSIGNED16 pbLocked;
AdsIsRecordLocked(hTable, 5, &pbLocked);
// pbLocked indica se o registo 5 está bloqueado
```

## Ver Também

- [AdsLockRecord]({{ site.baseurl }}/pt/funcoes/ads-lock-record/)
- [AdsUnlockRecord]({{ site.baseurl }}/pt/funcoes/ads-unlock-record/)
- [AdsIsTableLocked]({{ site.baseurl }}/pt/funcoes/ads-is-table-locked/)

---

[AdsLockTable →]({{ site.baseurl }}/pt/funcoes/ads-lock-table/)
