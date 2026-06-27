---
title: AdsDeleteCustomKey
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-delete-custom-key/
---

# AdsDeleteCustomKey

Exclui uma chave personalizada de um índice.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDeleteCustomKey(ADSHANDLE hIndex);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hIndex` | `ADSHANDLE` | Handle do índice. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDeleteCustomKey` remove a chave personalizada atual do índice. Índices personalizados permitem que o usuário adicione e remova chaves manualmente, em vez de serem geradas automaticamente pela expressão do índice.

## Exemplo

```c
AdsDeleteCustomKey(hIndex);
```

## Ver Também

- [AdsAddCustomKey]({{ site.baseurl }}/pt/funcoes/ads-add-custom-key/)
- [AdsCreateIndex]({{ site.baseurl }}/pt/funcoes/ads-create-index/)

---

[AdsDeleteIndex →]({{ site.baseurl }}/pt/funcoes/ads-delete-index/)
