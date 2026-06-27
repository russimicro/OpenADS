---
title: AdsEnableRI
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-enable-ri/
---

# AdsEnableRI

Ativa a integridade referencial.

## Sintaxe

```c
UNSIGNED32 AdsEnableRI(ADSHANDLE hConn);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConn` | `ADSHANDLE` | Handle da conexão. |

## Valor de Retorno

`AE_SUCCESS` (0) sempre.

## Descrição

`AdsEnableRI` ativa a integridade referencial. No OpenADS, é uma operação de no-op.

## Exemplo

```c
AdsEnableRI(hConnect);
```

## Ver Também

- [AdsDisableRI]({{ site.baseurl }}/pt/funcoes/ads-disable-ri/)
- [AdsCreateRefIntegrity]({{ site.baseurl }}/pt/funcoes/ads-create-ref-integrity/)
- [AdsDDCreateRefIntegrity]({{ site.baseurl }}/pt/funcoes/ads-dd-create-ref-integrity/)

---

[AdsDisableRI →]({{ site.baseurl }}/pt/funcoes/ads-disable-ri/)
