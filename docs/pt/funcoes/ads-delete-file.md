---
title: AdsDeleteFile
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-delete-file/
---

# AdsDeleteFile

Elimina um arquivo.

## Sintaxe

```c
UNSIGNED32 AdsDeleteFile(ADSHANDLE hConn, UNSIGNED8* pucName);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConn` | `ADSHANDLE` | Handle da conexão (reservado). |
| `pucName` | `UNSIGNED8*` | Nome do arquivo a eliminar. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_INTERNAL_ERROR` (5000) se o arquivo não existir.

## Descrição

`AdsDeleteFile` elimina um arquivo do sistema de arquivos.

## Exemplo

```c
AdsDeleteFile(hConn, "temp.dbf");
```

## Ver Também

- [AdsCheckExistence]({{ site.baseurl }}/pt/funcoes/ads-check-existence/)
- [AdsPackTable]({{ site.baseurl }}/pt/funcoes/ads-pack-table/)
- [AdsZapTable]({{ site.baseurl }}/pt/funcoes/ads-zap-table/)

---

[AdsFlushFileBuffers →]({{ site.baseurl }}/pt/funcoes/ads-flush-file-buffers/)
