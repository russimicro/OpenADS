---
title: AdsGetFieldW
layout: default
parent: Referência da API
nav_order: 4
permalink: /pt/funcoes/ads-get-field-w/
---

# AdsGetFieldW

Retorna o valor de um campo como string Wide (UTF-16LE).

## Sintaxe

```c
UNSIGNED32 AdsGetFieldW(ADSHANDLE hTable, UNSIGNED8* pucField,
                        UNSIGNED16* pucBufW, UNSIGNED32* pulLenW,
                        UNSIGNED16 usOption);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucField` | `UNSIGNED8*` | Nome ou índice ordinal (1-based) do campo. |
| `pucBufW` | `UNSIGNED16*` | Buffer de saída para a string Wide. |
| `pulLenW` | `UNSIGNED32*` | Tamanho do buffer em caracteres (entrada) e caracteres escritos (saída). |
| `usOption` | `UNSIGNED16` | Opções de leitura (ex: `ADS_READ_ALL_COLUMNS`). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsGetFieldW` retorna o conteúdo de um campo como string Wide (UTF-16LE). Isso é útil para trabalhar com dados que contêm caracteres Unicode. O campo de nome permanece ASCII, apenas o buffer de dados é Wide.

## Exemplo

```c
UNSIGNED16 awcBuf[256];
UNSIGNED32 ulLen = 256;
AdsGetFieldW(hTable, "Nome", awcBuf, &ulLen, 0);
```

## Ver Também

- [AdsGetField]({{ site.baseurl }}/pt/funcoes/ads-get-field/)
- [AdsGetStringW]({{ site.baseurl }}/pt/funcoes/ads-get-string-w/)
- [AdsSetStringW]({{ site.baseurl }}/pt/funcoes/ads-set-string-w/)

---

[AdsGetFTSIndexes →]({{ site.baseurl }}/pt/funcoes/ads-get-fts-indexes/)
