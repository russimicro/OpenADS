---
title: AdsMgDisconnect
layout: default
parent: Referência da API
nav_order: 13
permalink: /pt/funcoes/ads-mg-disconnect/
---

# AdsMgDisconnect

Fecha uma conexão de gerenciamento.

## Sintaxe

```c
UNSIGNED32 AdsMgDisconnect(ADSHANDLE hMg);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hMg` | `ADSHANDLE` | Handle da conexão de gerenciamento. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsMgDisconnect` fecha uma conexão de gerenciamento previamente aberta com `AdsMgConnect`.

## Exemplo

```c
AdsMgDisconnect(hMgmt);
```

## Ver Também

- [AdsMgConnect]({{ site.baseurl }}/pt/funcoes/ads-mg-connect/)
- [AdsMgGetActivityInfo]({{ site.baseurl }}/pt/funcoes/ads-mg-get-activity-info/)
- [AdsDisconnect]({{ site.baseurl }}/pt/funcoes/ads-disconnect/)

---

[AdsMgDumpInternalTables →]({{ site.baseurl }}/pt/funcoes/ads-mg-dump-internal-tables/)
