---
title: AdsGotoRecord
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-goto-record/
---

# AdsGotoRecord

Posiciona num registo específico pelo seu número.

## Sintaxe

```c
UNSIGNED32 AdsGotoRecord(ADSHANDLE hTable, UNSIGNED32 ulRecord);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `ulRecord` | `UNSIGNED32` | Número do registo (1-based). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido ou o registo não existir.

## Descrição

`AdsGotoRecord` move o cursor para o registo especificado pelo número. O número do registo é baseado em 1 (o primeiro registo tem número 1).

Se o número do registo for inválido (0 ou maior que o número de registos), a função retorna um erro.

## Exemplo

```c
AdsGotoRecord(hTable, 5);
// Agora o cursor está no quinto registo
```

## Ver Também

- [AdsGotoTop]({{ site.baseurl }}/pt/funcoes/ads-goto-top/)
- [AdsGotoBottom]({{ site.baseurl }}/pt/funcoes/ads-goto-bottom/)
- [AdsGetRecordNum]({{ site.baseurl }}/pt/funcoes/ads-get-record-num/)

---

[AdsSkip →]({{ site.baseurl }}/pt/funcoes/ads-skip/)
