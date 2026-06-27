---
title: AdsZapTable
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-zap-table/
---

# AdsZapTable

Remove todos os registos da tabela.

## Sintaxe

```c
UNSIGNED32 AdsZapTable(ADSHANDLE hTable);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsZapTable` remove fisicamente todos os registos da tabela, ficando apenas a estrutura. Esta operação é irreversível.

Para tabelas remotas, a operação é executada no servidor.

## Exemplo

```c
AdsZapTable(hTable);
// A tabela está vazia (apenas estrutura)
```

## Ver Também

- [AdsPackTable]({{ site.baseurl }}/pt/funcoes/ads-pack-table/)
- [AdsDeleteRecord]({{ site.baseurl }}/pt/funcoes/ads-delete-record/)
- [AdsGetRecordCount]({{ site.baseurl }}/pt/funcoes/ads-get-record-count/)

---

[AdsCopyTable →]({{ site.baseurl }}/pt/funcoes/ads-copy-table/)
