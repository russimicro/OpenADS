---
title: AdsFTSSearch
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-fts-search/
---

# AdsFTSSearch

Realiza uma busca de texto completo em uma tabela.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsFTSSearch(ADSHANDLE hConnect, UNSIGNED8* pucFile, UNSIGNED8* pucQuery, UNSIGNED32* paRecnos, UNSIGNED32* pulCount);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `pucFile` | `UNSIGNED8*` | Nome do arquivo ou índice FTS. |
| `pucQuery` | `UNSIGNED8*` | Consulta de texto completo. |
| `paRecnos` | `UNSIGNED32*` | Array para receber os números de registo. |
| `pulCount` | `UNSIGNED32*` | Ponteiro para receber a contagem de resultados. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsFTSSearch` executa uma busca de texto completo (Full-Text Search) na tabela especificada. A consulta pode usar operadores booleanos e curingas. Os números de registo dos resultados são armazenados no array fornecido.

## Exemplo

```c
UNSIGNED32 aRecnos[100];
UNSIGNED32 ulCount;

AdsFTSSearch(hConnect, "Documentos", "harbour AND openads", aRecnos, &ulCount);
printf("Encontrados %u registros\n", ulCount);
```

## Ver Também

- [AdsCreateFTSIndex]({{ site.baseurl }}/pt/funcoes/ads-create-fts-index/)
- [AdsSeek]({{ site.baseurl }}/pt/funcoes/ads-seek/)

---

[AdsGetBinary →]({{ site.baseurl }}/pt/funcoes/ads-get-binary/)
