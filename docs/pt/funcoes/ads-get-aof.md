---
title: AdsGetAOF
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-aof/
---

# AdsGetAOF

Retorna a expressão da AOF ativa.

## Sintaxe

```c
UNSIGNED32 AdsGetAOF(ADSHANDLE hTable, UNSIGNED8* pucFilter, UNSIGNED16* pusLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucFilter` | `UNSIGNED8*` | Buffer para receber a expressão. |
| `pusLen` | `UNSIGNED16*` | Ponteiro para o tamanho do buffer. Na saída, contém o comprimento. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsGetAOF` retorna a expressão da AOF atualmente ativa na tabela.

## Exemplo

```c
UNSIGNED8 szAOF[256];
UNSIGNED16 usLen = sizeof(szAOF);
AdsGetAOF(hTable, szAOF, &usLen);
// szAOF contém a expressão da AOF
```

## Ver Também

- [AdsSetAOF]({{ site.baseurl }}/pt/funcoes/ads-set-aof/)
- [AdsClearAOF]({{ site.baseurl }}/pt/funcoes/ads-clear-aof/)
- [AdsGetAOFOptLevel]({{ site.baseurl }}/pt/funcoes/ads-get-aof-opt-level/)

---

[AdsGetConnectionType →]({{ site.baseurl }}/pt/funcoes/ads-get-connection-type/)
