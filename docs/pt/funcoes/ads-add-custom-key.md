---
title: AdsAddCustomKey
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-add-custom-key/
---

# AdsAddCustomKey

Adiciona uma chave personalizada a um índice.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsAddCustomKey(ADSHANDLE hIndex);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hIndex` | `ADSHANDLE` | Handle do índice. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsAddCustomKey` adiciona a chave do registro atual ao índice personalizado. Índices personalizados permitem ao utilizador adicionar e remover chaves manualmente, em vez de serem geradas automaticamente pelo motor.

## Exemplo

```c
ADSHANDLE hIndex;
AdsAddCustomKey(hIndex);
```

## Ver Também

- [AdsDeleteCustomKey]({{ site.baseurl }}/pt/funcoes/ads-delete-custom-key/)

---

[AdsAggregate →]({{ site.baseurl }}/pt/funcoes/ads-aggregate/)
