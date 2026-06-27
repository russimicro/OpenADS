---
title: AdsStmtDisableEncryption
layout: default
parent: Referência da API
nav_order: 42
permalink: /pt/funcoes/ads-stmt-disable-encryption/
---

# AdsStmtDisableEncryption

Desabilita a criptografia para uma instrução SQL.

## Sintaxe

```c
UNSIGNED32 AdsStmtDisableEncryption(ADSHANDLE hStatement);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hStatement` | `ADSHANDLE` | Handle da instrução SQL. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsStmtDisableEncryption` desabilita a criptografia para tabelas acessadas pela instrução SQL. Isso pode ser útil quando se trabalha com tabelas criptografadas mas se deseja acessar os dados em texto plano.

## Exemplo

```c
AdsStmtDisableEncryption(hStatement);
AdsExecuteSQLDirect(hStatement, "SELECT * FROM Pedidos", &hCursor);
```

## Ver Também

- [AdsEnableEncryption]({{ site.baseurl }}/pt/funcoes/ads-enable-encryption/)
- [AdsIsEncryptionEnabled]({{ site.baseurl }}/pt/funcoes/ads-is-encryption-enabled/)
- [AdsExecuteSQL]({{ site.baseurl }}/pt/funcoes/ads-execute-sql/)

---

[AdsStmtSetTableCharType →]({{ site.baseurl }}/pt/funcoes/ads-stmt-set-table-char-type/)
