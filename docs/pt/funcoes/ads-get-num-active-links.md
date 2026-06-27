---
title: AdsGetNumActiveLinks
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-num-active-links/
---

# AdsGetNumActiveLinks

Retorna a contagem de ligações ativas do dicionário de dados.

## Sintaxe

```c
UNSIGNED32 AdsGetNumActiveLinks(ADSHANDLE hTable, UNSIGNED16 *pusCount);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pusCount` | `UNSIGNED16*` | Saída — número de ligações ativas. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso.

## Descrição

`AdsGetNumActiveLinks` retorna o número de ligações ativas do
dicionário de dados associadas à tabela dada. As ligações do
dicionário de dados definem relações entre tabelas e são utilizadas
para integridade referencial e otimização de consultas.

## Exemplo

```c
unsigned short numLinks = 0;
AdsGetNumActiveLinks(hTable, &numLinks);
printf("Ligações DD ativas: %u\n", numLinks);
```

## Ver Também

- [AdsGetNumOpenTables]({{ site.baseurl }}/pt/funcoes/ads-get-num-open-tables/)
- [AdsGetNumLocks]({{ site.baseurl }}/pt/funcoes/ads-get-num-locks/)
- [AdsGetTableConnection]({{ site.baseurl }}/pt/funcoes/ads-get-table-connection/)

---

[← AdsGetNumOpenTables]({{ site.baseurl }}/pt/funcoes/ads-get-num-open-tables/)
[AdsOpenTable →]({{ site.baseurl }}/pt/funcoes/ads-open-table/)
