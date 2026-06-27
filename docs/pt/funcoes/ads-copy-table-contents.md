---
title: AdsCopyTableContents
layout: default
parent: Referência da API
nav_order: 21
permalink: /pt/funcoes/ads-copy-table-contents/
---

# AdsCopyTableContents

Copia o conteúdo entre tabelas.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsCopyTableContents(ADSHANDLE hSrc, ADSHANDLE hDst,
                                            UNSIGNED16 usFilterOption);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hSrc` | `ADSHANDLE` | Handle da tabela de origem. |
| `hDst` | `ADSHANDLE` | Handle da tabela de destino. |
| `usFilterOption` | `UNSIGNED16` | Opção de filtro (respeitar filtros/escopos). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsCopyTableContents` copia os registros da tabela de origem para a tabela de destino. O filtro pode ser especificado para copiar apenas registros que atendam à condição.

## Exemplo

```c
AdsCopyTableContents(hSrc, hDst, ADS_RESPECTFILTERS);
```

## Ver Também

- [AdsCopyTable]({{ site.baseurl }}/pt/funcoes/ads-copy-table/)
- [AdsCopyTableStructure]({{ site.baseurl }}/pt/funcoes/ads-copy-table-structure/)

---

[AdsCreateFTSIndex →]({{ site.baseurl }}/pt/funcoes/ads-create-fts-index/)
