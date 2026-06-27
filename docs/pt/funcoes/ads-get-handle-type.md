---
title: AdsGetHandleType
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-handle-type/
---

# AdsGetHandleType

Retorna o tipo de um handle.

## Sintaxe

```c
UNSIGNED32 AdsGetHandleType(ADSHANDLE h, UNSIGNED16* p);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `h` | `ADSHANDLE` | Handle a verificar. |
| `p` | `UNSIGNED16*` | Ponteiro para receber o tipo do handle. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsGetHandleType` retorna o tipo de um handle:
- `ADS_NONE` (0) - Handle inválido
- `ADS_DATABASE_CONNECTION` (1) - Conexão
- `ADS_TABLE` (2) - Tabela
- `ADS_INDEX` (3) - Índice
- `ADS_STATEMENT` (4) - Statement SQL

## Exemplo

```c
UNSIGNED16 usType;
AdsGetHandleType(h, &usType);
// usType indica o tipo do handle
```

## Ver Também

- [AdsGetConnectionType]({{ site.baseurl }}/pt/funcoes/ads-get-connection-type/)
- [AdsGetTableType]({{ site.baseurl }}/pt/funcoes/ads-get-table-type/)
- [AdsGetNumOpenTables]({{ site.baseurl }}/pt/funcoes/ads-get-num-open-tables/)

---

[AdsGetLastError →]({{ site.baseurl }}/pt/funcoes/ads-get-last-error/)
