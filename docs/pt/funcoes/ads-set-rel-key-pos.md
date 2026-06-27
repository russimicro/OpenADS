---
title: AdsSetRelKeyPos
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-set-rel-key-pos/
---

# AdsSetRelKeyPos

Posiciona o cursor numa posição relativa.

## Sintaxe

```c
UNSIGNED32 AdsSetRelKeyPos(ADSHANDLE h, double pos);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `h` | `ADSHANDLE` | Handle da tabela ou índice. |
| `pos` | `double` | Posição relativa (0.0 a 1.0). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsSetRelKeyPos` posiciona o cursor numa posição relativa (0.0 = início, 1.0 = fim). Para índices ativos, navega pela ordem do índice.

## Exemplo

```c
AdsSetRelKeyPos(hTable, 0.5);  // Posiciona a meio
```

## Ver Também

- [AdsGetRelKeyPos]({{ site.baseurl }}/pt/funcoes/ads-get-rel-key-pos/)
- [AdsGotoRecord]({{ site.baseurl }}/pt/funcoes/ads-goto-record/)
- [AdsGetKeyNum]({{ site.baseurl }}/pt/funcoes/ads-get-key-num/)

---

[AdsGetKeyNum →]({{ site.baseurl }}/pt/funcoes/ads-get-key-num/)
