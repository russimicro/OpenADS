---
title: AdsGetNumLocks
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-num-locks/
---

# AdsGetNumLocks

Retorna a contagem de bloqueios de registo mantidos numa tabela.

## Sintaxe

```c
UNSIGNED32 AdsGetNumLocks(ADSHANDLE hTable, UNSIGNED16 *pusCount);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pusCount` | `UNSIGNED16*` | Saída — número de bloqueios de registo ativos. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso.

## Descrição

`AdsGetNumLocks` retorna quantos bloqueios de registo estão
atualmente mantidos na tabela especificada. Esta contagem inclui
tanto bloqueios explícitos (adquiridos através de `AdsLockRecord`)
como bloqueios implícitos adquiridos durante a navegação ou
atualizações.

## Exemplo

```c
unsigned short numLocks = 0;
AdsGetNumLocks(hTable, &numLocks);
printf("Bloqueios de registo ativos: %u\n", numLocks);
```

## Ver Também

- [AdsLockRecord]({{ site.baseurl }}/pt/funcoes/ads-lock-record/)
- [AdsUnlockRecord]({{ site.baseurl }}/pt/funcoes/ads-unlock-record/)
- [AdsGetAllLocks]({{ site.baseurl }}/pt/funcoes/ads-get-all-locks/)

---

[← AdsGetKeyNum]({{ site.baseurl }}/pt/funcoes/ads-get-key-num/)
[AdsGetNumOpenTables →]({{ site.baseurl }}/pt/funcoes/ads-get-num-open-tables/)
