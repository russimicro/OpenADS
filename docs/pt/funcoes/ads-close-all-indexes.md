---
title: AdsCloseAllIndexes
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-close-all-indexes/
---

# AdsCloseAllIndexes

Fecha todos os índices abertos de uma tabela.

## Sintaxe

```c
UNSIGNED32 AdsCloseAllIndexes(ADSHANDLE hTable);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsCloseAllIndexes` fecha todos os índices previamente abertos para a tabela especificada.

Para tabelas remotas, a operação é executada no servidor.

## Exemplo

```c
AdsCloseAllIndexes(hTable);
```

## Ver Também

- [AdsCloseIndex]({{ site.baseurl }}/pt/funcoes/ads-close-index/)
- [AdsOpenIndex]({{ site.baseurl }}/pt/funcoes/ads-open-index/)
- [AdsGetNumIndexes]({{ site.baseurl }}/pt/funcoes/ads-get-num-indexes/)

---

[AdsGetNumIndexes →]({{ site.baseurl }}/pt/funcoes/ads-get-num-indexes/)
