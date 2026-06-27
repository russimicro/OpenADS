---
title: AdsGetDefault
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-default/
---

# AdsGetDefault

Devolve o diretório padrão registado para a sessão.

## Sintaxe

```c
UNSIGNED32 AdsGetDefault(UNSIGNED8 *pucPath, UNSIGNED16 *pusLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `pucPath` | `UNSIGNED8*` | Buffer que recebe a cadeia do diretório padrão. |
| `pusLen` | `UNSIGNED16*` | Entrada/saída — tamanho do buffer à entrada; comprimento da cadeia à saída. |

## Valor de Retorno

`AE_SUCCESS` (0).

## Descrição

`AdsGetDefault` devolve a cadeia de diretório definida previamente com `AdsSetDefault`, ou uma cadeia vazia se nenhuma foi definida. As duas funções formam um round-trip; veja `AdsSetDefault` para como o OpenADS trata o valor.

## Exemplo

```c
UNSIGNED8 buf[260];
UNSIGNED16 len = sizeof(buf);
AdsGetDefault(buf, &len);
```

## Ver Também

- [AdsSetDefault]({{ site.baseurl }}/pt/funcoes/ads-set-default/)
- [AdsGetSearchPath]({{ site.baseurl }}/pt/funcoes/ads-get-search-path/)

---

[← AdsSetDefault]({{ site.baseurl }}/pt/funcoes/ads-set-default/)
