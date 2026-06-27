---
title: AdsGotoBottom
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-goto-bottom/
---

# AdsGotoBottom

Posiciona no último registo da tabela.

## Sintaxe

```c
UNSIGNED32 AdsGotoBottom(ADSHANDLE hTable);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela ou índice. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsGotoBottom` move o cursor para o último registo da tabela ou índice. Se a tabela estiver vazia, o cursor fica em estado EOF (End Of File).

Para tabelas remotas, a operação é executada no servidor e os dados são cacheados para acesso imediato.

## Exemplo

```c
AdsGotoBottom(hTable);
// Agora o cursor está no último registo
```

## Ver Também

- [AdsGotoTop]({{ site.baseurl }}/pt/funcoes/ads-goto-top/)
- [AdsGotoRecord]({{ site.baseurl }}/pt/funcoes/ads-goto-record/)
- [AdsSkip]({{ site.baseurl }}/pt/funcoes/ads-skip/)

---

[AdsGotoRecord →]({{ site.baseurl }}/pt/funcoes/ads-goto-record/)
