---
title: AdsDisableRI
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-disable-ri/
---

# AdsDisableRI

Desativa a integridade referencial.

## Sintaxe

```c
UNSIGNED32 AdsDisableRI(ADSHANDLE hConn);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConn` | `ADSHANDLE` | Handle da conexão. |

## Valor de Retorno

`AE_SUCCESS` (0) sempre.

## Descrição

`AdsDisableRI` desativa a integridade referencial. No OpenADS, é uma operação de no-op.

## Exemplo

```c
AdsDisableRI(hConnect);
```

## Ver Também

- [AdsEnableRI]({{ site.baseurl }}/pt/funcoes/ads-enable-ri/)
- [AdsCreateRefIntegrity]({{ site.baseurl }}/pt/funcoes/ads-create-ref-integrity/)
- [AdsRemoveRefIntegrity]({{ site.baseurl }}/pt/funcoes/ads-remove-ref-integrity/)

---

[AdsEnableUniqueEnforcement →]({{ site.baseurl }}/pt/funcoes/ads-enable-unique-enforcement/)
