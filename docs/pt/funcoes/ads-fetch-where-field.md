---
title: AdsFetchWhereField
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-fetch-where-field/
---

# AdsFetchWhereField

Obtém o valor de uma coluna de uma linha do resultado.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsFetchWhereField(ADSHANDLE hRes, UNSIGNED32 ulRow, UNSIGNED8* pszCol, UNSIGNED8* pucBuf, UNSIGNED16* pusLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hRes` | `ADSHANDLE` | Handle do resultado. |
| `ulRow` | `UNSIGNED32` | Índice da linha (base 0). |
| `pszCol` | `UNSIGNED8*` | Nome da coluna. |
| `pucBuf` | `UNSIGNED8*` | Buffer para receber o valor. |
| `pusLen` | `UNSIGNED16*` | Comprimento do buffer; retorna o comprimento do valor. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsFetchWhereField` recupera o valor de uma coluna específica de uma linha no conjunto de resultados retornado por AdsFetchWhere.

## Exemplo

```c
UNSIGNED16 usLen = 256;
UNSIGNED8 aucValue[256];

AdsFetchWhereField(hResult, 0, "Nome", aucValue, &usLen);
```

## Ver Também

- [AdsFetchWhere]({{ site.baseurl }}/pt/funcoes/ads-fetch-where/)
- [AdsFetchWhereRecno]({{ site.baseurl }}/pt/funcoes/ads-fetch-where-recno/)

---

[AdsFetchWhereRecno →]({{ site.baseurl }}/pt/funcoes/ads-fetch-where-recno/)
