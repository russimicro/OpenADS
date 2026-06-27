---
title: AdsFetchWhereRecno
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-fetch-where-recno/
---

# AdsFetchWhereRecno

Obtém o número de registo de uma linha do resultado.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsFetchWhereRecno(ADSHANDLE hRes, UNSIGNED32 ulRow, UNSIGNED32* pulRec);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hRes` | `ADSHANDLE` | Handle do resultado. |
| `ulRow` | `UNSIGNED32` | Índice da linha (base 0). |
| `pulRec` | `UNSIGNED32*` | Ponteiro para receber o número do registo. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsFetchWhereRecno` recupera o número do registo original de uma linha específica no conjunto de resultados. Esta função só funciona quando AdsFetchWhere foi chamada com a flag `WANT_RECNO` (0x01).

## Exemplo

```c
UNSIGNED32 ulRecno;

AdsFetchWhere(hTable, "Estado = 'AT'", "Nome", 100, 0x01, &hResult);
AdsFetchWhereRecno(hResult, 0, &ulRecno);
```

## Ver Também

- [AdsFetchWhere]({{ site.baseurl }}/pt/funcoes/ads-fetch-where/)
- [AdsFetchWhereField]({{ site.baseurl }}/pt/funcoes/ads-fetch-where-field/)

---

[AdsFetchWhereRows →]({{ site.baseurl }}/pt/funcoes/ads-fetch-where-rows/)
