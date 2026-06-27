---
title: AdsAppendRecord
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-append-record/
---

# AdsAppendRecord

Adiciona um novo registo vazio à tabela.

## Sintaxe

```c
UNSIGNED32 AdsAppendRecord(ADSHANDLE hTable);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsAppendRecord` adiciona um novo registo vazio ao final da tabela e posiciona o cursor nele. O registo é preenchido com valores por omissão (espaços para caracteres, zeros para numéricos, etc.).

Para tabelas remotas, a operação é executada no servidor e o cache de contagem de registos é invalidado.

## Exemplo

```c
AdsAppendRecord(hTable);
AdsSetField(hTable, "Nome", "João Silva", 10);
AdsWriteRecord(hTable);
```

## Ver Também

- [AdsWriteRecord]({{ site.baseurl }}/pt/funcoes/ads-write-record/)
- [AdsDeleteRecord]({{ site.baseurl }}/pt/funcoes/ads-delete-record/)
- [AdsRecallRecord]({{ site.baseurl }}/pt/funcoes/ads-recall-record/)

---

[AdsDeleteRecord →]({{ site.baseurl }}/pt/funcoes/ads-delete-record/)
