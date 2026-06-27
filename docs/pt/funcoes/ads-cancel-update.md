---
title: AdsCancelUpdate
layout: default
parent: Referência da API
nav_order: 11
permalink: /pt/funcoes/ads-cancel-update/
---

# AdsCancelUpdate

Cancela uma atualização pendente.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsCancelUpdate(ADSHANDLE hTable);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsCancelUpdate` cancela qualquer atualização pendente no registro atual, revertendo os campos para os valores originais antes da edição.

## Exemplo

```c
AdsCancelUpdate(hTable);
```

## Ver Também

- [AdsCancelUpdate90]({{ site.baseurl }}/pt/funcoes/ads-cancel-update-90/)
- [AdsWriteRecord]({{ site.baseurl }}/pt/funcoes/ads-write-record/)

---

[AdsCancelUpdate90 →]({{ site.baseurl }}/pt/funcoes/ads-cancel-update-90/)
