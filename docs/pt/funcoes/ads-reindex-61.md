---
title: AdsReindex61
layout: default
parent: Referência da API
nav_order: 33
permalink: /pt/funcoes/ads-reindex-61/
---

# AdsReindex61

Recria todos os índices da tabela com tamanho de página especificado.

## Sintaxe

```c
UNSIGNED32 AdsReindex61(ADSHANDLE hObject, UNSIGNED32 ulPageSize);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hObject` | `ADSHANDLE` | Handle da tabela ou índice. |
| `ulPageSize` | `UNSIGNED32` | Tamanho da página do índice (0 para padrão). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsReindex61` recria todos os índices associados à tabela. Essa variante (versão 61) permite especificar o tamanho da página do índice, o que pode melhorar o desempenho para tabelas grandes. O valor 0 usa o tamanho padrão de 512 bytes.

## Exemplo

```c
AdsReindex61(hTable, 1024);
// Todos os índices são recriados com páginas de 1024 bytes
```

## Ver Também

- [AdsReindex]({{ site.baseurl }}/pt/funcoes/ads-reindex/)
- [AdsCreateIndex61]({{ site.baseurl }}/pt/funcoes/ads-create-index-61/)
- [AdsGetNumIndexes]({{ site.baseurl }}/pt/funcoes/ads-get-num-indexes/)

---

[AdsResetConnection →]({{ site.baseurl }}/pt/funcoes/ads-reset-connection/)
