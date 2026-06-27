---
title: AdsPackTable
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-pack-table/
---

# AdsPackTable

Remove fisicamente os registos eliminados.

## Sintaxe

```c
UNSIGNED32 AdsPackTable(ADSHANDLE hTable);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsPackTable` remove fisicamente todos os registos marcados como eliminados da tabela. Esta operação é irreversível.

Para tabelas remotas, a operação é executada no servidor.

## Exemplo

```c
AdsPackTable(hTable);
// Registos eliminados foram removidos fisicamente
```

## Ver Também

- [AdsDeleteRecord]({{ site.baseurl }}/pt/funcoes/ads-delete-record/)
- [AdsRecallRecord]({{ site.baseurl }}/pt/funcoes/ads-recall-record/)
- [AdsZapTable]({{ site.baseurl }}/pt/funcoes/ads-zap-table/)

---

[AdsZapTable →]({{ site.baseurl }}/pt/funcoes/ads-zap-table/)
