---
title: AdsConvertAnsiToOem
layout: default
parent: Referência da API
nav_order: 19
permalink: /pt/funcoes/ads-convert-ansi-to-oem/
---

# AdsConvertAnsiToOem

Converte caracteres de ANSI para OEM.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsConvertAnsiToOem(UNSIGNED8* pucBuf,
                                           UNSIGNED32* pulLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `pucBuf` | `UNSIGNED8*` | Buffer com os caracteres a converter (in-place). |
| `pulLen` | `UNSIGNED32*` | Comprimento do buffer. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsConvertAnsiToOem` converte caracteres de uma string do conjunto de caracteres ANSI para OEM (DOS) in-place.

## Exemplo

```c
UNSIGNED32 ulLen = strlen(buffer);
AdsConvertAnsiToOem(buffer, &ulLen);
```

## Ver Também

- [AdsConvertOemToAnsi]({{ site.baseurl }}/pt/funcoes/ads-convert-oem-to-ansi/)

---

[AdsConvertOemToAnsi →]({{ site.baseurl }}/pt/funcoes/ads-convert-oem-to-ansi/)
