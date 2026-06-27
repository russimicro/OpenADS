---
title: AdsFetchWhereClose
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-fetch-where-close/
---

# AdsFetchWhereClose

Fecha um handle de resultado do AdsFetchWhere.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsFetchWhereClose(ADSHANDLE hRes);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hRes` | `ADSHANDLE` | Handle do resultado a ser fechado. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsFetchWhereClose` libera um handle de resultado retornado por AdsFetchWhere. Após o fechamento, o handle não é mais válido e não deve ser usado.

## Exemplo

```c
AdsFetchWhereClose(hResult);
```

## Ver Também

- [AdsFetchWhere]({{ site.baseurl }}/pt/funcoes/ads-fetch-where/)
- [AdsFetchWhereRows]({{ site.baseurl }}/pt/funcoes/ads-fetch-where-rows/)

---

[AdsFetchWhereEof →]({{ site.baseurl }}/pt/funcoes/ads-fetch-where-eof/)
