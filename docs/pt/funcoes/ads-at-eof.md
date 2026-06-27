---
title: AdsAtEOF
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-at-eof/
---

# AdsAtEOF

Verifica se o cursor está após o último registo.

## Sintaxe

```c
UNSIGNED32 AdsAtEOF(ADSHANDLE hTable, UNSIGNED16* pbAtEnd);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pbAtEnd` | `UNSIGNED16*` | Ponteiro para receber 1 se estiver no EOF, 0 caso contrário. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsAtEOF` verifica se o cursor está após o último registo da tabela (EOF - End Of File). O estado EOF ocorre quando:
- A tabela está vazia
- O cursor foi movido para além do último registo com `AdsSkip`

Para tabelas remotas, se houver um registo válido em cache, a função retorna 0 sem comunicação com o servidor. Isto permite que ciclos de navegação sequencial usem dados prefetch sem round-trips adicionais.

## Exemplo

```c
AdsGotoBottom(hTable);
AdsSkip(hTable, 1);
if (pbAtEnd) {
    // O cursor está no final da tabela
}
```

## Ver Também

- [AdsAtBOF]({{ site.baseurl }}/pt/funcoes/ads-at-bof/)
- [AdsGotoBottom]({{ site.baseurl }}/pt/funcoes/ads-goto-bottom/)
- [AdsSkip]({{ site.baseurl }}/pt/funcoes/ads-skip/)

---

[AdsGetNumFields →]({{ site.baseurl }}/pt/funcoes/ads-get-num-fields/)
