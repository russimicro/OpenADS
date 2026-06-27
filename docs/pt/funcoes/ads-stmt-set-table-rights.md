---
title: AdsStmtSetTableRights
layout: default
parent: Referência da API
nav_order: 45
permalink: /pt/funcoes/ads-stmt-set-table-rights/
---

# AdsStmtSetTableRights

Define o nível de verificação de direitos para tabelas acessadas por uma instrução SQL.

## Sintaxe

```c
UNSIGNED32 AdsStmtSetTableRights(ADSHANDLE hStatement, UNSIGNED16 usCheckRights);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hStatement` | `ADSHANDLE` | Handle da instrução SQL. |
| `usCheckRights` | `UNSIGNED16` | Nível de verificação de direitos. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsStmtSetTableRights` define se a instrução SQL deve verificar os direitos de acesso do usuário ao acessar tabelas. Use `ADS_CHECKRIGHTS` (1) para habilitar a verificação ou 0 para desabilitar.

## Exemplo

```c
AdsStmtSetTableRights(hStatement, ADS_CHECKRIGHTS);
```

## Ver Também

- [AdsOpenTable]({{ site.baseurl }}/pt/funcoes/ads-open-table/)
- [AdsCreateTable]({{ site.baseurl }}/pt/funcoes/ads-create-table/)
- [AdsExecuteSQL]({{ site.baseurl }}/pt/funcoes/ads-execute-sql/)

---

[AdsStudioPort →]({{ site.baseurl }}/pt/funcoes/ads-studio-port/)
