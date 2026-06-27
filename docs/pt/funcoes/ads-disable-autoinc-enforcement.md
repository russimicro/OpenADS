---
title: AdsDisableAutoIncEnforcement
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-disable-autoinc-enforcement/
---

# AdsDisableAutoIncEnforcement

Desativa a imposição de autoincremento.

## Sintaxe

```c
UNSIGNED32 AdsDisableAutoIncEnforcement(ADSHANDLE hConn);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConn` | `ADSHANDLE` | Handle da conexão. |

## Valor de Retorno

`AE_SUCCESS` (0) sempre.

## Descrição

`AdsDisableAutoIncEnforcement` desativa a imposição de autoincremento. No OpenADS, é uma operação de no-op.

## Exemplo

```c
AdsDisableAutoIncEnforcement(hConnect);
```

## Ver Também

- [AdsEnableAutoIncEnforcement]({{ site.baseurl }}/pt/funcoes/ads-enable-autoinc-enforcement/)
- [AdsGetLastAutoinc]({{ site.baseurl }}/pt/funcoes/ads-get-last-autoinc/)
- [AdsCreateTable]({{ site.baseurl }}/pt/funcoes/ads-create-table/)

---

[AdsRecallAllRecords →]({{ site.baseurl }}/pt/funcoes/ads-recall-all-records/)
