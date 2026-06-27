---
title: AdsSkip
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-skip/
---

# AdsSkip

Move o cursor para frente ou para trás um número especificado de registos.

## Sintaxe

```c
UNSIGNED32 AdsSkip(ADSHANDLE hTable, SIGNED32 lRows);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela ou índice. |
| `lRows` | `SIGNED32` | Número de registos a saltar (positivo para frente, negativo para trás). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsSkip` move o cursor para frente ou para trás o número especificado de registos. Se o salto ultrapassar os limites da tabela, o cursor fica em estado BOF ou EOF conforme apropriado.

Para tabelas remotas, o OpenADS implementa prefetch sequencial: quando `lRows` é 1, usa dados cacheados da operação anterior, resultando em zero RTT para cada passo em cache.

## Exemplo

```c
AdsGotoTop(hTable);
AdsSkip(hTable, 5);   // Avança 5 registos
AdsSkip(hTable, -2);  // Volta 2 registos
```

## Ver Também

- [AdsGotoTop]({{ site.baseurl }}/pt/funcoes/ads-goto-top/)
- [AdsGotoBottom]({{ site.baseurl }}/pt/funcoes/ads-goto-bottom/)
- [AdsAtBOF]({{ site.baseurl }}/pt/funcoes/ads-at-bof/)
- [AdsAtEOF]({{ site.baseurl }}/pt/funcoes/ads-at-eof/)

---

[AdsAtBOF →]({{ site.baseurl }}/pt/funcoes/ads-at-bof/)
