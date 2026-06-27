---
title: AdsConvertOemToAnsi
layout: default
parent: Referência da API
nav_order: 20
permalink: /pt/funcoes/ads-convert-oem-to-ansi/
---

# AdsConvertOemToAnsi

Converte caracteres de OEM para ANSI.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsConvertOemToAnsi(UNSIGNED8* pucBuf,
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

`AdsConvertOemToAnsi` converte caracteres de uma string do conjunto de caracteres OEM (DOS) para ANSI in-place.

## Exemplo

```c
UNSIGNED32 ulLen = strlen(buffer);
AdsConvertOemToAnsi(buffer, &ulLen);
```

## Ver Também

- [AdsConvertAnsiToOem]({{ site.baseurl }}/pt/funcoes/ads-convert-ansi-to-oem/)

---

[AdsCopyTableContents →]({{ site.baseurl }}/pt/funcoes/ads-copy-table-contents/)
