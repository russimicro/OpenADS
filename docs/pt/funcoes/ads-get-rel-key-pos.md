---
title: AdsGetRelKeyPos
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-rel-key-pos/
---

# AdsGetRelKeyPos

Retorna a posição relativa da chave.

## Sintaxe

```c
UNSIGNED32 AdsGetRelKeyPos(ADSHANDLE h, double* p);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `h` | `ADSHANDLE` | Handle da tabela ou índice. |
| `p` | `double*` | Ponteiro para receber a posição relativa (0.0 a 1.0). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsGetRelKeyPos` retorna a posição relativa do cursor (0.0 = início, 1.0 = fim). Útil para barras de deslocamento.

## Exemplo

```c
double pos;
AdsGetRelKeyPos(hTable, &pos);
// pos contém a posição relativa (0.0 a 1.0)
```

## Ver Também

- [AdsSetRelKeyPos]({{ site.baseurl }}/pt/funcoes/ads-set-rel-key-pos/)
- [AdsGetKeyNum]({{ site.baseurl }}/pt/funcoes/ads-get-key-num/)
- [AdsGetKeyCount]({{ site.baseurl }}/pt/funcoes/ads-get-key-count/)

---

[AdsSetRelKeyPos →]({{ site.baseurl }}/pt/funcoes/ads-set-rel-key-pos/)
