---
title: AdsWriteRecord
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-write-record/
---

# AdsWriteRecord

Grava as alterações do registo atual.

## Sintaxe

```c
UNSIGNED32 AdsWriteRecord(ADSHANDLE hTable);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsWriteRecord` grava no disco todas as alterações feitas ao registo atual. As alterações aos campos são feitas em memória com `AdsSetField`, `AdsSetString`, etc., e só são persistidas quando `AdsWriteRecord` é chamado.

Para tabelas remotas, a operação é executada no servidor e o cache de linha é invalidado.

## Exemplo

```c
AdsSetField(hTable, "Nome", "João Silva", 10);
AdsWriteRecord(hTable);
```

## Ver Também

- [AdsSetField]({{ site.baseurl }}/pt/funcoes/ads-set-field/)
- [AdsSetString]({{ site.baseurl }}/pt/funcoes/ads-set-string/)
- [AdsAppendRecord]({{ site.baseurl }}/pt/funcoes/ads-append-record/)

---

[AdsAppendRecord →]({{ site.baseurl }}/pt/funcoes/ads-append-record/)
