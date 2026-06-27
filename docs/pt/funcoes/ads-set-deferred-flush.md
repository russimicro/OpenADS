---
title: AdsSetDeferredFlush
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-set-deferred-flush/
---

# AdsSetDeferredFlush

Define o modo de flush diferido.

## Sintaxe

```c
UNSIGNED32 AdsSetDeferredFlush(ADSHANDLE hTable, UNSIGNED16 usDeferred);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `usDeferred` | `UNSIGNED16` | 1 para ativar flush diferido, 0 para desativar. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsSetDeferredFlush` define se as escritas devem ser diferidas (agrupadas) ou imediatas.

## Exemplo

```c
AdsSetDeferredFlush(hTable, 1);  // Ativar flush diferido
// Operações de escrita...
AdsFlushFileBuffers(hTable);  // Forçar escrita
```

## Ver Também

- [AdsFlushFileBuffers]({{ site.baseurl }}/pt/funcoes/ads-flush-file-buffers/)
- [AdsWriteRecord]({{ site.baseurl }}/pt/funcoes/ads-write-record/)
- [AdsCloseTable]({{ site.baseurl }}/pt/funcoes/ads-close-table/)

---

[AdsGetRecordCRC →]({{ site.baseurl }}/pt/funcoes/ads-get-record-crc/)
