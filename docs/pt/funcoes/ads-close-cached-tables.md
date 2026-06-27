---
title: AdsCloseCachedTables
layout: default
parent: Referência da API
nav_order: 10
permalink: /pt/funcoes/ads-close-cached-tables/
---

# AdsCloseCachedTables

Fecha todas as tabelas em cache.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsCloseCachedTables(ADSHANDLE hConnect);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsCloseCachedTables` fecha todas as tabelas que estão no cache para a conexão especificada. Útil para liberar recursos quando as tabelas não estão mais sendo utilizadas.

## Exemplo

```c
AdsCloseCachedTables(hConnect);
```

## Ver Também

- [AdsCacheOpenCursors]({{ site.baseurl }}/pt/funcoes/ads-cache-open-cursors/)
- [AdsCacheOpenTables]({{ site.baseurl }}/pt/funcoes/ads-cache-open-tables/)
- [AdsCacheRecords]({{ site.baseurl }}/pt/funcoes/ads-cache-records/)

---

[AdsCancelUpdate →]({{ site.baseurl }}/pt/funcoes/ads-cancel-update/)
