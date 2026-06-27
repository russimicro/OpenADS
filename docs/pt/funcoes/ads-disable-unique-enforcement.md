---
title: AdsDisableUniqueEnforcement
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-disable-unique-enforcement/
---

# AdsDisableUniqueEnforcement

Desativa a imposição de unicidade.

## Sintaxe

```c
UNSIGNED32 AdsDisableUniqueEnforcement(ADSHANDLE hConn);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConn` | `ADSHANDLE` | Handle da conexão. |

## Valor de Retorno

`AE_SUCCESS` (0) sempre.

## Descrição

`AdsDisableUniqueEnforcement` desativa a imposição de unicidade para índices. No OpenADS, é uma operação de no-op.

## Exemplo

```c
AdsDisableUniqueEnforcement(hConnect);
```

## Ver Também

- [AdsEnableUniqueEnforcement]({{ site.baseurl }}/pt/funcoes/ads-enable-unique-enforcement/)
- [AdsDisableAutoIncEnforcement]({{ site.baseurl }}/pt/funcoes/ads-disable-autoinc-enforcement/)
- [AdsCreateIndex]({{ site.baseurl }}/pt/funcoes/ads-create-index/)

---

[AdsEnableAutoIncEnforcement →]({{ site.baseurl }}/pt/funcoes/ads-enable-autoinc-enforcement/)
