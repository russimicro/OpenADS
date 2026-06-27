---
title: AdsGetRecordCount
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-record-count/
---

# AdsGetRecordCount

Retorna o número de registos da tabela.

## Sintaxe

```c
UNSIGNED32 AdsGetRecordCount(ADSHANDLE hTable, UNSIGNED16 bFilterOption,
                             UNSIGNED32* pulRecordCount);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela ou índice. |
| `bFilterOption` | `UNSIGNED16` | Opção de filtro (0 para todos, 1 para visíveis). |
| `pulRecordCount` | `UNSIGNED32*` | Ponteiro para receber o número de registos. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsGetRecordCount` retorna o número de registos da tabela. Para tabelas remotas, o valor é cacheado e servido sem comunicação com o servidor.

Se um handle de índice for passado, a função retorna o número de registos acessíveis através desse índice (útil para índices com condição FOR).

## Exemplo

```c
UNSIGNED32 ulCount;
AdsGetRecordCount(hTable, 0, &ulCount);
// ulCount contém o número de registos
```

## Ver También

- [AdsGetNumFields]({{ site.baseurl }}/pt/funcoes/ads-get-num-fields/)
- [AdsGetKeyCount]({{ site.baseurl }}/pt/funcoes/ads-get-key-count/)
- [AdsGetRecordNum]({{ site.baseurl }}/pt/funcoes/ads-get-record-num/)

---

[AdsGetRecordNum →]({{ site.baseurl }}/pt/funcoes/ads-get-record-num/)
