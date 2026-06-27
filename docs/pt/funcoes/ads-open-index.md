---
title: AdsOpenIndex
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-open-index/
---

# AdsOpenIndex

Abre um índice existente.

## Sintaxe

```c
UNSIGNED32 AdsOpenIndex(ADSHANDLE hTable, UNSIGNED8* pucName,
                        ADSHANDLE* ahIndex, UNSIGNED16* pu16ArrayLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucName` | `UNSIGNED8*` | Nome do arquivo de índice ou tag. |
| `ahIndex` | `ADSHANDLE*` | Array para receber os handles dos índices abertos. |
| `pu16ArrayLen` | `UNSIGNED16*` | Tamanho do array. Na saída, contém o número de índices abertos. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se a tabela for desconhecida.

## Descrição

`AdsOpenIndex` abre um arquivo de índice existente e retorna handles para cada tag (índice) contido nele.

Para tabelas remotas, a operação é executada no servidor.

## Exemplo

```c
ADSHANDLE arr[64];
UNSIGNED16 usLen = 64;
AdsOpenIndex(hTable, "dados.cdx", arr, &usLen);
// arr contém os handles dos índices abertos
```

## Ver Também

- [AdsCloseIndex]({{ site.baseurl }}/pt/funcoes/ads-close-index/)
- [AdsGetIndexHandle]({{ site.baseurl }}/pt/funcoes/ads-get-index-handle/)
- [AdsGetNumIndexes]({{ site.baseurl }}/pt/funcoes/ads-get-num-indexes/)

---

[AdsCloseIndex →]({{ site.baseurl }}/pt/funcoes/ads-close-index/)
