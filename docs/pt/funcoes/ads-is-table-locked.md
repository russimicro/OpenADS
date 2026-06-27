---
title: AdsIsTableLocked
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-is-table-locked/
---

# AdsIsTableLocked

Verifica se uma tabela tem bloqueio exclusivo.

## Sintaxe

```c
UNSIGNED32 AdsIsTableLocked(ADSHANDLE hTable, UNSIGNED16 *pbLocked);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pbLocked` | `UNSIGNED16*` | Saída — `ADS_TRUE` se a tabela tiver bloqueio exclusivo, `ADS_FALSE` caso contrário. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso.

## Descrição

`AdsIsTableLocked` verifica se a tabela especificada tem um
bloqueio exclusivo mantido. Um bloqueio exclusivo impede que
outros utilizadores acedam à tabela. Isto é diferente dos bloqueios
de registo individuais, que apenas bloqueiam registos específicos.

## Exemplo

```c
unsigned short bLocked = 0;
AdsIsTableLocked(hTable, &bLocked);
if (bLocked == ADS_TRUE)
    printf("A tabela tem bloqueio exclusivo\n");
else
    printf("A tabela não tem bloqueio exclusivo\n");
```

## Ver Também

- [AdsLockTable]({{ site.baseurl }}/pt/funcoes/ads-lock-table/)
- [AdsUnlockTable]({{ site.baseurl }}/pt/funcoes/ads-unlock-table/)
- [AdsGetNumLocks]({{ site.baseurl }}/pt/funcoes/ads-get-num-locks/)

---

[← AdsIsRecordLocked]({{ site.baseurl }}/pt/funcoes/ads-is-record-locked/)
[AdsLockRecord →]({{ site.baseurl }}/pt/funcoes/ads-lock-record/)
