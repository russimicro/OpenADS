---
title: AdsContinue
layout: default
parent: Referência da API
nav_order: 18
permalink: /pt/funcoes/ads-continue/
---

# AdsContinue

Continua uma busca localizando o próximo registro.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsContinue(ADSHANDLE hTable, UNSIGNED16* pbFound);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pbFound` | `UNSIGNED16*` | Ponteiro para receber se o registro foi encontrado (1=sim, 0=não). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsContinue` continua a busca iniciada por `AdsSeek`, localizando o próximo registro que atende à condição de filtro/escopo ativo.

## Exemplo

```c
UNSIGNED16 bFound;
AdsContinue(hTable, &bFound);
```

## Ver Também

- [AdsSeek]({{ site.baseurl }}/pt/funcoes/ads-seek/)
- [AdsSeekLast]({{ site.baseurl }}/pt/funcoes/ads-seek-last/)

---

[AdsConvertAnsiToOem →]({{ site.baseurl }}/pt/funcoes/ads-convert-ansi-to-oem/)
