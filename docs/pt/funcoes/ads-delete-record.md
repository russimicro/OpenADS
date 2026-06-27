---
title: AdsDeleteRecord
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-delete-record/
---

# AdsDeleteRecord

Marca o registo atual como eliminado.

## Sintaxe

```c
UNSIGNED32 AdsDeleteRecord(ADSHANDLE hTable);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsDeleteRecord` marca o registo atual como eliminado. O registo não é fisicamente removido até que `AdsPackTable` seja chamado.

Para restaurar o registo eliminado, use `AdsRecallRecord`.

## Exemplo

```c
AdsDeleteRecord(hTable);
// O registo está marcado como eliminado
AdsRecallRecord(hTable);  // Restaura o registo
```

## Ver Também

- [AdsRecallRecord]({{ site.baseurl }}/pt/funcoes/ads-recall-record/)
- [AdsIsRecordDeleted]({{ site.baseurl }}/pt/funcoes/ads-is-record-deleted/)
- [AdsPackTable]({{ site.baseurl }}/pt/funcoes/ads-pack-table/)

---

[AdsRecallRecord →]({{ site.baseurl }}/pt/funcoes/ads-recall-record/)
