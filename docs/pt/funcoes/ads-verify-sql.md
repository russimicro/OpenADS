---
title: AdsVerifySQL
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-verify-sql/
---

# AdsVerifySQL

Verifica se uma consulta SQL é válida.

## Sintaxe

```c
UNSIGNED32 AdsVerifySQL(ADSHANDLE hStatement, UNSIGNED8* pucSQL);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hStatement` | `ADSHANDLE` | Handle do statement (reservado). |
| `pucSQL` | `UNSIGNED8*` | Consulta SQL a verificar. |

## Valor de Retorno

`AE_SUCCESS` (0) se a consulta for válida. `AE_PARSE_ERROR` se houver erro de sintaxe.

## Descrição

`AdsVerifySQL` verifica se uma consulta SQL tem sintaxe válida sem a executar.

## Exemplo

```c
UNSIGNED32 rc = AdsVerifySQL(hStmt, "SELECT * FROM clientes");
if (rc == AE_SUCCESS) {
    // SQL válido
}
```

## Ver Também

- [AdsExecuteSQLDirect]({{ site.baseurl }}/pt/funcoes/ads-execute-sql-direct/)
- [AdsCreateSQLStatement]({{ site.baseurl }}/pt/funcoes/ads-create-sql-statement/)
- [AdsCloseSQLStatement]({{ site.baseurl }}/pt/funcoes/ads-close-sql-statement/)

---

[AdsCloseSQLStatement →]({{ site.baseurl }}/pt/funcoes/ads-close-sql-statement/)
