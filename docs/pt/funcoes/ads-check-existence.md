---
title: AdsCheckExistence
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-check-existence/
---

# AdsCheckExistence

Verifica se um arquivo existe.

## Sintaxe

```c
UNSIGNED32 AdsCheckExistence(ADSHANDLE hConn, UNSIGNED8* pucName,
                             UNSIGNED16* pbExists);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConn` | `ADSHANDLE` | Handle da conexão (reservado). |
| `pucName` | `UNSIGNED8*` | Nome do arquivo. |
| `pbExists` | `UNSIGNED16*` | Ponteiro para receber 1 se existir, 0 caso contrário. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o ponteiro for nulo.

## Descrição

`AdsCheckExistence` verifica se um arquivo existe no sistema de arquivos.

## Exemplo

```c
UNSIGNED16 pbExists;
AdsCheckExistence(hConn, "clientes.dbf", &pbExists);
if (pbExists) {
    // O arquivo existe
}
```

## Ver Também

- [AdsOpenTable]({{ site.baseurl }}/pt/funcoes/ads-open-table/)
- [AdsDeleteFile]({{ site.baseurl }}/pt/funcoes/ads-delete-file/)
- [AdsFindFirstTable]({{ site.baseurl }}/pt/funcoes/ads-find-first-table/)

---

[AdsDeleteFile →]({{ site.baseurl }}/pt/funcoes/ads-delete-file/)
