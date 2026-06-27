---
title: AdsGetString
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-string/
---

# AdsGetString

Retorna o valor de um campo como string terminada em NUL.

## Sintaxe

```c
UNSIGNED32 AdsGetString(ADSHANDLE hTable, UNSIGNED8* pucField,
                        UNSIGNED8* pucBuf, UNSIGNED32* pulLen,
                        UNSIGNED16 usOption);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucField` | `UNSIGNED8*` | Nome do campo ou ordinal (via ADSFIELD). |
| `pucBuf` | `UNSIGNED8*` | Buffer para receber a string. |
| `pulLen` | `UNSIGNED32*` | Ponteiro para o tamanho do buffer. Na saída, contém o comprimento da string. |
| `usOption` | `UNSIGNED16` | Opção reservada (usar 0). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_COLUMN_NOT_FOUND` (6125) se o campo não for encontrado.

## Descrição

`AdsGetString` retorna o valor de um campo como string terminada em NUL. Ao contrário de `AdsGetField`, esta função remove espaços à direita (trailing spaces).

Para tabelas remotas, a função serve do cache de linha após a primeira navegação.

## Exemplo

```c
UNSIGNED8 szValue[256];
UNSIGNED32 ulLen = sizeof(szValue);
AdsGetString(hTable, "Nome", szValue, &ulLen, 0);
// szValue contém "João Silva" (sem espaços à direita)
```

## Ver Também

- [AdsGetField]({{ site.baseurl }}/pt/funcoes/ads-get-field/)
- [AdsGetStringW]({{ site.baseurl }}/pt/funcoes/ads-get-string-w/)
- [AdsSetString]({{ site.baseurl }}/pt/funcoes/ads-set-string/)

---

[AdsSetString →]({{ site.baseurl }}/pt/funcoes/ads-set-string/)
