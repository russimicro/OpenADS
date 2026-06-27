---
title: AdsCacheRecords
layout: default
parent: Referência da API
nav_order: 9
permalink: /pt/funcoes/ads-cache-records/
---

# AdsCacheRecords

Configura o cache de registros.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsCacheRecords(ADSHANDLE hTable,
                                      UNSIGNED16 usRecCount);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `usRecCount` | `UNSIGNED16` | Número de registros a manter em cache. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsCacheRecords` define quantos registros o motor deve manter em cache para a tabela especificada. O cache de registros melhora o desempenho de navegação sequencial.

## Exemplo

```c
AdsCacheRecords(hTable, 100);
```

## Ver Também

- [AdsCacheOpenCursors]({{ site.baseurl }}/pt/funcoes/ads-cache-open-cursors/)
- [AdsCacheOpenTables]({{ site.baseurl }}/pt/funcoes/ads-cache-open-tables/)
- [AdsCloseCachedTables]({{ site.baseurl }}/pt/funcoes/ads-close-cached-tables/)

---

[AdsCloseCachedTables →]({{ site.baseurl }}/pt/funcoes/ads-close-cached-tables/)
