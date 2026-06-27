---
title: AdsDDFindFirstObject
layout: default
parent: Referência da API
nav_order: 48
permalink: /pt/funcoes/ads-dd-find-first-object/
---

# AdsDDFindFirstObject

Encontra o primeiro objeto no dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDFindFirstObject(ADSHANDLE   hObject,
                                           UNSIGNED16  usFindObjectType,
                                           UNSIGNED8*  pucParentName,
                                           UNSIGNED8*  pucObjectName,
                                           UNSIGNED16* pusObjectNameLen,
                                           ADSHANDLE*  phFindHandle);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hObject` | `ADSHANDLE` | Handle do dicionário de dados. |
| `usFindObjectType` | `UNSIGNED16` | Tipo do objeto a buscar. |
| `pucParentName` | `UNSIGNED8*` | Nome do objeto pai (pode ser NULL). |
| `pucObjectName` | `UNSIGNED8*` | Buffer para receber o nome do objeto. |
| `pusObjectNameLen` | `UNSIGNED16*` | Comprimento do buffer/nome retornado. |
| `phFindHandle` | `ADSHANDLE*` | Ponteiro para receber o handle da busca. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDFindFirstObject` inicia uma busca por objetos no dicionário de dados. Retorna o primeiro objeto encontrado do tipo especificado. Use `AdsDDFindNextObject` para iterar sobre os demais objetos, e `AdsDDFindClose` para liberar os recursos.

## Exemplo

```c
ADSHANDLE hFind;
UNSIGNED16 usLen = 255;
UNSIGNED8 aucName[256];
AdsDDFindFirstObject(hDD, ADS_TABLE, NULL, aucName, &usLen, &hFind);
```

## Ver Também

- [AdsDDFindNextObject]({{ site.baseurl }}/pt/funcoes/ads-dd-find-next-object/)
- [AdsDDFindClose]({{ site.baseurl }}/pt/funcoes/ads-dd-find-close/)

---

[AdsDDFindNextObject →]({{ site.baseurl }}/pt/funcoes/ads-dd-find-next-object/)
