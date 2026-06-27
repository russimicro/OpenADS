---
title: AdsGetKeyNum
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-key-num/
---

# AdsGetKeyNum

Retorna a posição lógica da chave atual.

## Sintaxe

```c
UNSIGNED32 AdsGetKeyNum(ADSHANDLE hObj, UNSIGNED16 usFilterOption,
                        UNSIGNED32* pulKeyNum);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hObj` | `ADSHANDLE` | Handle da tabela ou índice. |
| `usFilterOption` | `UNSIGNED16` | Opção de filtro (reservada). |
| `pulKeyNum` | `UNSIGNED32*` | Ponteiro para receber o número da chave. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsGetKeyNum` retorna a posição lógica do registo atual na ordem do índice (1-based). Sem índice ativo, retorna o número do registo.

## Exemplo

```c
UNSIGNED32 ulKeyNum;
AdsGetKeyNum(hTable, 0, &ulKeyNum);
// ulKeyNum contém a posição lógica
```

## Ver Também

- [AdsGetKeyCount]({{ site.baseurl }}/pt/funcoes/ads-get-key-count/)
- [AdsGetRecordNum]({{ site.baseurl }}/pt/funcoes/ads-get-record-num/)
- [AdsSetIndexOrder]({{ site.baseurl }}/pt/funcoes/ads-set-index-order/)

---

[AdsSetIndexOrder →]({{ site.baseurl }}/pt/funcoes/ads-set-index-order/)
