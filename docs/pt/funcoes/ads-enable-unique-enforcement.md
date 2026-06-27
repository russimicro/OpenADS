---
title: AdsEnableUniqueEnforcement
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-enable-unique-enforcement/
---

# AdsEnableUniqueEnforcement

Ativa a imposição de unicidade.

## Sintaxe

```c
UNSIGNED32 AdsEnableUniqueEnforcement(ADSHANDLE hConn);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConn` | `ADSHANDLE` | Handle da conexão. |

## Valor de Retorno

`AE_SUCCESS` (0) sempre.

## Descrição

`AdsEnableUniqueEnforcement` ativa a imposição de unicidade para índices. No OpenADS, é uma operação de no-op.

## Exemplo

```c
AdsEnableUniqueEnforcement(hConnect);
```

## Ver Também

- [AdsDisableUniqueEnforcement]({{ site.baseurl }}/pt/funcoes/ads-disable-unique-enforcement/)
- [AdsEnableAutoIncEnforcement]({{ site.baseurl }}/pt/funcoes/ads-enable-autoinc-enforcement/)
- [AdsCreateIndex]({{ site.baseurl }}/pt/funcoes/ads-create-index/)

---

[AdsDisableUniqueEnforcement →]({{ site.baseurl }}/pt/funcoes/ads-disable-unique-enforcement/)
