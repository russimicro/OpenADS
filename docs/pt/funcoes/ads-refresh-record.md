---
title: AdsRefreshRecord
layout: default
parent: Referência da API
nav_order: 30
permalink: /pt/funcoes/ads-refresh-record/
---

# AdsRefreshRecord

Recarrega o registro atual do servidor.

## Sintaxe

```c
UNSIGNED32 AdsRefreshRecord(ADSHANDLE hTable);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsRefreshRecord` recarrega o registro atual do servidor, descartando quaisquer alterações não gravadas feitas no cache local. Isso é útil quando se suspeita que outro usuário modificou o registro.

## Exemplo

```c
AdsRefreshRecord(hTable);
// Agora o registro contém os dados mais recentes do servidor
```

## Ver Também

- [AdsWriteRecord]({{ site.baseurl }}/pt/funcoes/ads-write-record/)
- [AdsGetRecord]({{ site.baseurl }}/pt/funcoes/ads-get-record/)
- [AdsGotoRecord]({{ site.baseurl }}/pt/funcoes/ads-goto-record/)

---

[AdsRegisterCallbackFunction →]({{ site.baseurl }}/pt/funcoes/ads-register-callback-function/)
