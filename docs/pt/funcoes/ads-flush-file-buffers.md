---
title: AdsFlushFileBuffers
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-flush-file-buffers/
---

# AdsFlushFileBuffers

Força a escrita dos buffers para disco.

## Sintaxe

```c
UNSIGNED32 AdsFlushFileBuffers(ADSHANDLE hTable);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsFlushFileBuffers` força a escrita de todos os buffers pendentes para disco.

Para tabelas remotas, a operação é executada no servidor.

## Exemplo

```c
AdsFlushFileBuffers(hTable);
```

## Ver Também

- [AdsWriteRecord]({{ site.baseurl }}/pt/funcoes/ads-write-record/)
- [AdsSetDeferredFlush]({{ site.baseurl }}/pt/funcoes/ads-set-deferred-flush/)
- [AdsCloseTable]({{ site.baseurl }}/pt/funcoes/ads-close-table/)

---

[AdsSetDeferredFlush →]({{ site.baseurl }}/pt/funcoes/ads-set-deferred-flush/)
