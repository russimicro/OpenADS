---
title: AdsSetAOF
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-set-aof/
---

# AdsSetAOF

Define uma Advantage Optimized Filter (AOF).

## Sintaxe

```c
UNSIGNED32 AdsSetAOF(ADSHANDLE hTable, UNSIGNED8* pucCondition,
                     UNSIGNED16 usResolve);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucCondition` | `UNSIGNED8*` | Condição do filtro. |
| `usResolve` | `UNSIGNED16` | Opção de resolução (reservada). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se a condição for nula.

## Descrição

`AdsSetAOF` define uma AOF que pode ser otimizada pelo motor de índices. Se a expressão não puder ser otimizada, o filtro é aplicado client-side.

Para backends SQL, a condição é traduzida para SQL WHEN possível.

## Exemplo

```c
AdsSetAOF(hTable, "Idade > 18 AND Ativo = .T.", 0);
```

## Ver Também

- [AdsClearAOF]({{ site.baseurl }}/pt/funcoes/ads-clear-aof/)
- [AdsGetAOF]({{ site.baseurl }}/pt/funcoes/ads-get-aof/)
- [AdsGetAOFOptLevel]({{ site.baseurl }}/pt/funcoes/ads-get-aof-opt-level/)

---

[AdsClearAOF →]({{ site.baseurl }}/pt/funcoes/ads-clear-aof/)
