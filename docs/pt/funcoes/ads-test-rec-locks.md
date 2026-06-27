---
title: AdsTestRecLocks
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-test-rec-locks/
---

# AdsTestRecLocks

Testa a consistência dos bloqueios de registos.

## Sintaxe

```c
UNSIGNED32 AdsTestRecLocks(ADSHANDLE hTable);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsTestRecLocks` é um hook de diagnóstico que valida o handle da tabela.

## Exemplo

```c
UNSIGNED32 rc = AdsTestRecLocks(hTable);
if (rc == AE_SUCCESS) {
    // Tabela válida
}
```

## Ver Também

- [AdsIsTableLocked]({{ site.baseurl }}/pt/funcoes/ads-is-table-locked/)
- [AdsIsRecordLocked]({{ site.baseurl }}/pt/funcoes/ads-is-record-locked/)
- [AdsLockTable]({{ site.baseurl }}/pt/funcoes/ads-lock-table/)

---

[AdsGetLockCycle →]({{ site.baseurl }}/pt/funcoes/ads-get-lock-cycle/)
