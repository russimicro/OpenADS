---
title: AdsCacheOpenTables
layout: default
parent: Referência da API
nav_order: 8
permalink: /pt/funcoes/ads-cache-open-tables/
---

# AdsCacheOpenTables

Configura o cache de tabelas abertas.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsCacheOpenTables(UNSIGNED16 usCacheCount);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `usCacheCount` | `UNSIGNED16` | Número máximo de tabelas a manter em cache. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsCacheOpenTables` define quantas tabelas o motor deve manter em cache para melhorar o desempenho. O cache reduz a sobrecarga de abrir e fechar tabelas frequentemente.

## Exemplo

```c
AdsCacheOpenTables(5);
```

## Ver Também

- [AdsCacheOpenCursors]({{ site.baseurl }}/pt/funcoes/ads-cache-open-cursors/)
- [AdsCacheRecords]({{ site.baseurl }}/pt/funcoes/ads-cache-records/)
- [AdsCloseCachedTables]({{ site.baseurl }}/pt/funcoes/ads-close-cached-tables/)

---

[AdsCacheRecords →]({{ site.baseurl }}/pt/funcoes/ads-cache-records/)
