---
title: AdsLockRecord
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-lock-record/
---

# AdsLockRecord

Bloqueia um registo específico.

## Sintaxe

```c
UNSIGNED32 AdsLockRecord(ADSHANDLE hTable, UNSIGNED32 ulRecord);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `ulRecord` | `UNSIGNED32` | Número do registo a bloquear (0 para o atual). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsLockRecord` bloqueia um registo específico para edição exclusiva. Se `ulRecord` for 0, o registo atual é bloqueado.

Para tabelas remotas, o bloqueio é gerido pelo servidor.

## Exemplo

```c
AdsLockRecord(hTable, 5);  // Bloqueia o registo 5
// Editar o registo...
AdsUnlockRecord(hTable, 5);  // Desbloqueia
```

## Ver Também

- [AdsUnlockRecord]({{ site.baseurl }}/pt/funcoes/ads-unlock-record/)
- [AdsIsRecordLocked]({{ site.baseurl }}/pt/funcoes/ads-is-record-locked/)
- [AdsLockTable]({{ site.baseurl }}/pt/funcoes/ads-lock-table/)

---

[AdsUnlockRecord →]({{ site.baseurl }}/pt/funcoes/ads-unlock-record/)
