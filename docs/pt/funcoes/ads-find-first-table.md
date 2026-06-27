---
title: AdsFindFirstTable
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-find-first-table/
---

# AdsFindFirstTable

Encontra a primeira tabela que corresponde à máscara.

## Sintaxe

```c
UNSIGNED32 AdsFindFirstTable(ADSHANDLE   hConnect,
                             UNSIGNED8*  pucMask,
                             UNSIGNED8*  pucFileName,
                             UNSIGNED16* pusFileNameLen,
                             ADSHANDLE*  phFind);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `pucMask` | `UNSIGNED8*` | Máscara de procura (ex: "*.dbf"). |
| `pucFileName` | `UNSIGNED8*` | Buffer para receber o nome do arquivo. |
| `pusFileNameLen` | `UNSIGNED16*` | Ponteiro para o tamanho do buffer. |
| `phFind` | `ADSHANDLE*` | Ponteiro para receber o handle de procura. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_NO_FILE_FOUND` se nenhuma tabela for encontrada.

## Descrição

`AdsFindFirstTable` encontra a primeira tabela que corresponde à máscara na pasta de dados da conexão.

## Exemplo

```c
ADSHANDLE hFind;
UNSIGNED8 szFile[256];
UNSIGNED16 usLen = sizeof(szFile);
AdsFindFirstTable(hConnect, "*.dbf", szFile, &usLen, &hFind);
// szFile contém o nome da primeira tabela
```

## Ver Também

- [AdsFindNextTable]({{ site.baseurl }}/pt/funcoes/ads-find-next-table/)
- [AdsFindClose]({{ site.baseurl }}/pt/funcoes/ads-find-close/)
- [AdsCheckExistence]({{ site.baseurl }}/pt/funcoes/ads-check-existence/)

---

[AdsFindNextTable →]({{ site.baseurl }}/pt/funcoes/ads-find-next-table/)
