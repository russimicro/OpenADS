---
title: AdsAtBOF
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-at-bof/
---

# AdsAtBOF

Verifica se o cursor está antes do primeiro registo.

## Sintaxe

```c
UNSIGNED32 AdsAtBOF(ADSHANDLE hTable, UNSIGNED16* pbAtBegin);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pbAtBegin` | `UNSIGNED16*` | Ponteiro para receber 1 se estiver no BOF, 0 caso contrário. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsAtBOF` verifica se o cursor está antes do primeiro registo da tabela (BOF - Before Of File). O estado BOF ocorre quando:
- A tabela está vazia
- O cursor foi movido para antes do primeiro registo com `AdsSkip`

Para tabelas remotas, se houver um registo válido em cache, a função retorna 0 sem comunicação com o servidor.

## Exemplo

```c
AdsGotoTop(hTable);
AdsSkip(hTable, -1);
if (pbAtBegin) {
    // O cursor está no início da tabela
}
```

## Ver Também

- [AdsAtEOF]({{ site.baseurl }}/pt/funcoes/ads-at-eof/)
- [AdsGotoTop]({{ site.baseurl }}/pt/funcoes/ads-goto-top/)
- [AdsSkip]({{ site.baseurl }}/pt/funcoes/ads-skip/)

---

[AdsAtEOF →]({{ site.baseurl }}/pt/funcoes/ads-at-eof/)
