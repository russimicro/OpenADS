---
title: AdsInitRawKey
layout: default
parent: Referência da API
nav_order: 9
permalink: /pt/funcoes/ads-init-raw-key/
---

# AdsInitRawKey

Inicializa um índice para busca com chaves raw.

## Sintaxe

```c
UNSIGNED32 AdsInitRawKey(ADSHANDLE hIndex);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hIndex` | `ADSHANDLE` | Handle do índice. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsInitRawKey` prepara um índice para operações de busca usando chaves em formato raw (bytes brutos). Isso é necessário antes de usar `AdsSeek` com chaves raw em índices que não são de string.

## Exemplo

```c
AdsInitRawKey(hIndex);
// Agora é possível buscar com chaves raw
```

## Ver Também

- [AdsSeek]({{ site.baseurl }}/pt/funcoes/ads-seek/)
- [AdsGetKeyType]({{ site.baseurl }}/pt/funcoes/ads-get-key-type/)
- [AdsExtractKey]({{ site.baseurl }}/pt/funcoes/ads-extract-key/)

---

[AdsIsEmpty →]({{ site.baseurl }}/pt/funcoes/ads-is-empty/)
