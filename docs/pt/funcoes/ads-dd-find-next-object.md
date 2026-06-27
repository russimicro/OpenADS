---
title: AdsDDFindNextObject
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-dd-find-next-object/
---

# AdsDDFindNextObject

Encontra o próximo objeto no dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDFindNextObject(ADSHANDLE hObject, ADSHANDLE hFindHandle, UNSIGNED8* pucObjectName, UNSIGNED16* pusObjectNameLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hObject` | `ADSHANDLE` | Handle da conexão ou tabela. |
| `hFindHandle` | `ADSHANDLE` | Handle de busca retornado por AdsDDFindFirstObject. |
| `pucObjectName` | `UNSIGNED8*` | Buffer para receber o nome do objeto. |
| `pusObjectNameLen` | `UNSIGNED16*` | Comprimento do buffer; retorna o comprimento do nome. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDFindNextObject` localiza o próximo objeto no dicionário de dados que corresponde aos critérios de busca iniciados por AdsDDFindFirstObject. O handle de busca (`hFindHandle`) deve ser válido e ter sido retornado por uma chamada anterior a AdsDDFindFirstObject.

## Exemplo

```c
ADSHANDLE hFind;
UNSIGNED16 usLen = 255;
UNSIGNED8 aucName[256];

AdsDDFindFirstObject(hConnect, ADS_DD_TABLE, NULL, aucName, &usLen, &hFind);
// Processar o primeiro objeto...
while (AdsDDFindNextObject(hConnect, hFind, aucName, &usLen) == AE_SUCCESS) {
    // Processar cada objeto encontrado
}
AdsDDFindClose(hConnect, hFind);
```

## Ver Também

- [AdsDDFindFirstObject]({{ site.baseurl }}/pt/funcoes/ads-dd-find-first-object/)
- [AdsDDFindClose]({{ site.baseurl }}/pt/funcoes/ads-dd-find-close/)

---

[AdsDDFindClose →]({{ site.baseurl }}/pt/funcoes/ads-dd-find-close/)
