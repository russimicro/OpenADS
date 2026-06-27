---
title: AdsEnableAutoIncEnforcement
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-enable-autoinc-enforcement/
---

# AdsEnableAutoIncEnforcement

Ativa a imposição de autoincremento.

## Sintaxe

```c
UNSIGNED32 AdsEnableAutoIncEnforcement(ADSHANDLE hConn);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConn` | `ADSHANDLE` | Handle da conexão. |

## Valor de Retorno

`AE_SUCCESS` (0) sempre.

## Descrição

`AdsEnableAutoIncEnforcement` ativa a imposição de autoincremento. No OpenADS, é uma operação de no-op.

## Exemplo

```c
AdsEnableAutoIncEnforcement(hConnect);
```

## Ver Também

- [AdsDisableAutoIncEnforcement]({{ site.baseurl }}/pt/funcoes/ads-disable-autoinc-enforcement/)
- [AdsGetLastAutoinc]({{ site.baseurl }}/pt/funcoes/ads-get-last-autoinc/)
- [AdsCreateTable]({{ site.baseurl }}/pt/funcoes/ads-create-table/)

---

[AdsDisableAutoIncEnforcement →]({{ site.baseurl }}/pt/funcoes/ads-disable-autoinc-enforcement/)
