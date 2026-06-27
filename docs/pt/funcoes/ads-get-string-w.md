---
title: AdsGetStringW
layout: default
parent: Referência da API
nav_order: 8
permalink: /pt/funcoes/ads-get-string-w/
---

# AdsGetStringW

Retorna o valor de um campo de caracteres como string Wide (UTF-16LE).

## Sintaxe

```c
UNSIGNED32 AdsGetStringW(ADSHANDLE hTable, UNSIGNED8* pucField,
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
| `usOption` | `UNSIGNED16` | Opções de leitura (0 = padrão). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsGetStringW` retorna o conteúdo de um campo de caracteres como string Wide (UTF-16 preservando o BOM). Útil para dados Unicode.

## Exemplo

```c
UNSIGNED16 awcBuf[256];
UNSIGNED32 ulLen = 256;
AdsGetStringW(hTable, "Nome", awcBuf, &ulLen, 0);
```

## Ver Também

- [AdsGetString]({{ site.baseurl }}/pt/funcoes/ads-get-string/)
- [AdsGetFieldW]({{ site.baseurl }}/pt/funcoes/ads-get-field-w/)
- [AdsSetStringW]({{ site.baseurl }}/pt/funcoes/ads-set-string-w/)

---

[AdsInitRawKey →]({{ site.baseurl }}/pt/funcoes/ads-init-raw-key/)
