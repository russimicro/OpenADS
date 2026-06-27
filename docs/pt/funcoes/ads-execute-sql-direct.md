---
title: AdsExecuteSQLDirect
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-execute-sql-direct/
---

# AdsExecuteSQLDirect

Executa uma consulta SQL diretamente.

## Sintaxe

```c
UNSIGNED32 AdsExecuteSQLDirect(ADSHANDLE hStatement, UNSIGNED8* pucSQL,
                               ADSHANDLE* phCursor);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hStatement` | `ADSHANDLE` | Handle do statement. |
| `pucSQL` | `UNSIGNED8*` | Consulta SQL a executar. |
| `phCursor` | `ADSHANDLE*` | Ponteiro para receber o handle do cursor. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsExecuteSQLDirect` executa uma consulta SQL e retorna um cursor com os resultados.

## Exemplo

```c
ADSHANDLE hCursor;
AdsExecuteSQLDirect(hStmt, "SELECT * FROM clientes WHERE ativo = 1", &hCursor);
```

## Ver Também

- [AdsCreateSQLStatement]({{ site.baseurl }}/pt/funcoes/ads-create-sql-statement/)
- [AdsCloseSQLStatement]({{ site.baseurl }}/pt/funcoes/ads-close-sql-statement/)
- [AdsVerifySQL]({{ site.baseurl }}/pt/funcoes/ads-verify-sql/)

---

[AdsVerifySQL →]({{ site.baseurl }}/pt/funcoes/ads-verify-sql/)
