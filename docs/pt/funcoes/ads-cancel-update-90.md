---
title: AdsCancelUpdate90
layout: default
parent: Referência da API
nav_order: 12
permalink: /pt/funcoes/ads-cancel-update-90/
---

# AdsCancelUpdate90

Cancela uma atualização pendente (versão 9.0+).

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsCancelUpdate90(ADSHANDLE hTable, UNSIGNED32 ulOptions);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `ulOptions` | `UNSIGNED32` | Opções adicionais (reservado). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsCancelUpdate90` é a versão estendida de `AdsCancelUpdate`, suportando opções adicionais introduzidas na versão 9.0 do ACE.

## Exemplo

```c
AdsCancelUpdate90(hTable, 0);
```

## Ver Também

- [AdsCancelUpdate]({{ site.baseurl }}/pt/funcoes/ads-cancel-update/)
- [AdsWriteRecord]({{ site.baseurl }}/pt/funcoes/ads-write-record/)

---

[AdsClearCallbackFunction →]({{ site.baseurl }}/pt/funcoes/ads-clear-callback-function/)
