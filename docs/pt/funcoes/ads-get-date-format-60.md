---
title: AdsGetDateFormat60
layout: default
parent: Referência da API
nav_order: 2
permalink: /pt/funcoes/ads-get-date-format-60/
---

# AdsGetDateFormat60

Retorna o formato de data ativo para uma conexão específica.

## Sintaxe

```c
UNSIGNED32 AdsGetDateFormat60(ADSHANDLE hConnect, UNSIGNED8* pucBuf,
                              UNSIGNED16* pusLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `pucBuf` | `UNSIGNED8*` | Buffer de saída para o formato de data. |
| `pusLen` | `UNSIGNED16*` | Tamanho do buffer (entrada) e bytes escritos (saída). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsGetDateFormat60` retorna a string de formato de data que está ativa na conexão especificada. Essa variante (versão 60) permite obter o formato de data de uma conexão particular, ao contrário de `AdsGetDateFormat` que retorna o formato global.

## Exemplo

```c
UNSIGNED8 aucFormat[64];
UNSIGNED16 usLen = sizeof(aucFormat);
AdsGetDateFormat60(hConnect, aucFormat, &usLen);
// aucFormat contém "MM/DD/YYYY"
```

## Ver Também

- [AdsGetDateFormat]({{ site.baseurl }}/pt/funcoes/ads-get-date-format/)
- [AdsSetDateFormat]({{ site.baseurl }}/pt/funcoes/ads-set-date-format/)
- [AdsGetDate]({{ site.baseurl }}/pt/funcoes/ads-get-date/)

---

[AdsGetExact22 →]({{ site.baseurl }}/pt/funcoes/ads-get-exact-22/)
