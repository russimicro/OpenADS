---
title: AdsUnlockRecord
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-unlock-record/
---

# AdsUnlockRecord

Desbloqueia um registo específico.

## Sintaxe

```c
UNSIGNED32 AdsUnlockRecord(ADSHANDLE hTable, UNSIGNED32 ulRecord);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `ulRecord` | `UNSIGNED32` | Número do registo a desbloquear (0 para o atual). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsUnlockRecord` desbloqueia um registo previamente bloqueado com `AdsLockRecord`.

Para tabelas remotas, o desbloqueio é gerido pelo servidor.

## Exemplo

```c
AdsUnlockRecord(hTable, 5);  // Desbloqueia o registo 5
```

## Ver Também

- [AdsLockRecord]({{ site.baseurl }}/pt/funcoes/ads-lock-record/)
- [AdsIsRecordLocked]({{ site.baseurl }}/pt/funcoes/ads-is-record-locked/)
- [AdsUnlockTable]({{ site.baseurl }}/pt/funcoes/ads-unlock-table/)

---

[AdsIsRecordLocked →]({{ site.baseurl }}/pt/funcoes/ads-is-record-locked/)
