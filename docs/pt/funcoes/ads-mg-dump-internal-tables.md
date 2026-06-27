---
title: AdsMgDumpInternalTables
layout: default
parent: Referência da API
nav_order: 14
permalink: /pt/funcoes/ads-mg-dump-internal-tables/
---

# AdsMgDumpInternalTables

Despeja as tabelas internas do servidor para diagnóstico.

## Sintaxe

```c
UNSIGNED32 AdsMgDumpInternalTables(ADSHANDLE hMgmtHandle);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hMgmtHandle` | `ADSHANDLE` | Handle da conexão de gerenciamento. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsMgDumpInternalTables` despeja informações sobre as tabelas internas do servidor. Essa função é usada para diagnóstico e solução de problemas do servidor.

## Exemplo

```c
AdsMgDumpInternalTables(hMgmt);
```

## Ver Também

- [AdsMgConnect]({{ site.baseurl }}/pt/funcoes/ads-mg-connect/)
- [AdsMgGetConfigInfo]({{ site.baseurl }}/pt/funcoes/ads-mg-get-config-info/)
- [AdsMgGetActivityInfo]({{ site.baseurl }}/pt/funcoes/ads-mg-get-activity-info/)

---

[AdsMgGetActivityInfo →]({{ site.baseurl }}/pt/funcoes/ads-mg-get-activity-info/)
