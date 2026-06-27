---
title: AdsFetchWhereEof
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-fetch-where-eof/
---

# AdsFetchWhereEof

Verifica se o cursor do resultado está no fim.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsFetchWhereEof(ADSHANDLE hRes, UNSIGNED16* pbEof);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hRes` | `ADSHANDLE` | Handle do resultado. |
| `pbEof` | `UNSIGNED16*` | Ponteiro para receber o valor booleano (verdadeiro se no fim). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsFetchWhereEof` verifica se o cursor de navegação do resultado está no fim (EOF). Esta função é usada em conjunto com as outras funções de AdsFetchWhere para iterar sobre os resultados.

## Exemplo

```c
UNSIGNED16 bEof;

AdsFetchWhereEof(hResult, &bEof);
while (!bEof) {
    // Processar linha atual
    AdsFetchWhereEof(hResult, &bEof);
}
```

## Ver Também

- [AdsFetchWhere]({{ site.baseurl }}/pt/funcoes/ads-fetch-where/)
- [AdsFetchWhereField]({{ site.baseurl }}/pt/funcoes/ads-fetch-where-field/)

---

[AdsFetchWhereField →]({{ site.baseurl }}/pt/funcoes/ads-fetch-where-field/)
