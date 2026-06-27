---
title: AdsGotoTop
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-goto-top/
---

# AdsGotoTop

Posiciona no primeiro registo da tabela.

## Sintaxe

```c
UNSIGNED32 AdsGotoTop(ADSHANDLE hTable);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela ou índice. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsGotoTop` move o cursor para o primeiro registo da tabela ou índice. Se a tabela estiver vazia, o cursor fica em estado BOF (Before Of File).

Para tabelas remotas, a operação é executada no servidor e os dados são cacheados para acesso imediato.

## Exemplo

```c
AdsGotoTop(hTable);
// Agora o cursor está no primeiro registo
```

## Ver Também

- [AdsGotoBottom]({{ site.baseurl }}/pt/funcoes/ads-goto-bottom/)
- [AdsGotoRecord]({{ site.baseurl }}/pt/funcoes/ads-goto-record/)
- [AdsSkip]({{ site.baseurl }}/pt/funcoes/ads-skip/)

---

[AdsGotoBottom →]({{ site.baseurl }}/pt/funcoes/ads-goto-bottom/)
