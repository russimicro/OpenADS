---
title: AdsCopyTableContent
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-copy-table-content/
---

# AdsCopyTableContent

Cópia o conteúdo de uma tabela para outra.

## Sintaxe

```c
UNSIGNED32 AdsCopyTableContent(ADSHANDLE hSrc, ADSHANDLE hDst);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hSrc` | `ADSHANDLE` | Handle da tabela origem. |
| `hDst` | `ADSHANDLE` | Handle da tabela destino. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se os handles forem desconhecidos.

## Descrição

`AdsCopyTableContent` copia todos os registos visíveis de uma tabela para outra. Campos com o mesmo nome são copiados; campos diferentes são ignorados.

## Exemplo

```c
AdsCopyTableContent(hOrigem, hDestino);
```

## Ver Também

- [AdsCopyTable]({{ site.baseurl }}/pt/funcoes/ads-copy-table/)
- [AdsCopyTableStructure]({{ site.baseurl }}/pt/funcoes/ads-copy-table-structure/)
- [AdsAppendRecord]({{ site.baseurl }}/pt/funcoes/ads-append-record/)

---

[AdsConvertTable →]({{ site.baseurl }}/pt/funcoes/ads-convert-table/)
