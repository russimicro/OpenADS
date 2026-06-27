---
title: AdsGetAOFOptLevel
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-aof-opt-level/
---

# AdsGetAOFOptLevel

Retorna o nível de otimização da AOF.

## Sintaxe

```c
UNSIGNED32 AdsGetAOFOptLevel(ADSHANDLE hTable, UNSIGNED16* pusLevel,
                             UNSIGNED8* pucBuf, UNSIGNED16* pusLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pusLevel` | `UNSIGNED16*` | Ponteiro para receber o nível de otimização. |
| `pucBuf` | `UNSIGNED8*` | Buffer para expressão não otimizada (reservado). |
| `pusLen` | `UNSIGNED16*` | Tamanho do buffer (reservado). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o handle for desconhecido.

## Descrição

`AdsGetAOFOptLevel` retorna o nível de otimização da AOF:
- `ADS_OPTIMIZED_NONE` (0) - Sem otimização
- `ADS_OPTIMIZED_PARTIAL` (1) - Otimização parcial
- `ADS_OPTIMIZED_FULL` (2) - Otimização completa

## Exemplo

```c
UNSIGNED16 usLevel;
AdsGetAOFOptLevel(hTable, &usLevel, nullptr, nullptr);
// usLevel indica o nível de otimização
```

## Ver Também

- [AdsSetAOF]({{ site.baseurl }}/pt/funcoes/ads-set-aof/)
- [AdsGetAOF]({{ site.baseurl }}/pt/funcoes/ads-get-aof/)
- [AdsCustomizeAOF]({{ site.baseurl }}/pt/funcoes/ads-customize-aof/)

---

[AdsRefreshAOF →]({{ site.baseurl }}/pt/funcoes/ads-refresh-aof/)
