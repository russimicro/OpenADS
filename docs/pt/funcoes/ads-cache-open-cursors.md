---
title: AdsCacheOpenCursors
layout: default
parent: Referência da API
nav_order: 7
permalink: /pt/funcoes/ads-cache-open-cursors/
---

# AdsCacheOpenCursors

Configura o cache de cursores abertos.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsCacheOpenCursors(UNSIGNED16 usCacheCount);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `usCacheCount` | `UNSIGNED16` | Número máximo de cursores a manter em cache. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsCacheOpenCursors` define quantos cursores o motor deve manter em cache para melhorar o desempenho. O cache reduz a sobrecarga de abrir e fechar cursores frequentemente.

## Exemplo

```c
AdsCacheOpenCursors(10);
```

## Ver Também

- [AdsCacheOpenTables]({{ site.baseurl }}/pt/funcoes/ads-cache-open-tables/)
- [AdsCacheRecords]({{ site.baseurl }}/pt/funcoes/ads-cache-records/)
- [AdsCloseCachedTables]({{ site.baseurl }}/pt/funcoes/ads-close-cached-tables/)

---

[AdsCacheOpenTables →]({{ site.baseurl }}/pt/funcoes/ads-cache-open-tables/)
