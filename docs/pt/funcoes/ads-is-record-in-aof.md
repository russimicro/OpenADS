---
title: AdsIsRecordInAOF
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-is-record-in-aof/
---

# AdsIsRecordInAOF

Verifica se um registo está incluído na AOF.

## Sintaxe

```c
UNSIGNED32 AdsIsRecordInAOF(ADSHANDLE hTable, UNSIGNED32 ulRecord,
                            UNSIGNED16* pbIsIn);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `ulRecord` | `UNSIGNED32` | Número do registo. |
| `pbIsIn` | `UNSIGNED16*` | Ponteiro para receber 1 se estiver na AOF, 0 caso contrário. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsIsRecordInAOF` verifica se um registo específico está incluído na AOF ativa.

**Nota:** No OpenADS, esta função sempre retorna 1 (o registo está na AOF).

## Exemplo

```c
UNSIGNED16 pbIsIn;
AdsIsRecordInAOF(hTable, 5, &pbIsIn);
// pbIsIn indica se o registo 5 está na AOF
```

## Ver Também

- [AdsSetAOF]({{ site.baseurl }}/pt/funcoes/ads-set-aof/)
- [AdsCustomizeAOF]({{ site.baseurl }}/pt/funcoes/ads-customize-aof/)
- [AdsIsRecordVisible]({{ site.baseurl }}/pt/funcoes/ads-is-record-visible/)

---

[AdsIsRecordVisible →]({{ site.baseurl }}/pt/funcoes/ads-is-record-visible/)
