---
title: AdsFindNextTable
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-find-next-table/
---

# AdsFindNextTable

Encontra a próxima tabela que corresponde à máscara.

## Sintaxe

```c
UNSIGNED32 AdsFindNextTable(ADSHANDLE   hConnect,
                            ADSHANDLE   hFind,
                            UNSIGNED8*  pucFileName,
                            UNSIGNED16* pusFileNameLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `hFind` | `ADSHANDLE` | Handle de procura de `AdsFindFirstTable`. |
| `pucFileName` | `UNSIGNED8*` | Buffer para receber o nome do arquivo. |
| `pusFileNameLen` | `UNSIGNED16*` | Ponteiro para o tamanho do buffer. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_NO_FILE_FOUND` se não houver mais tabelas.

## Descrição

`AdsFindNextTable` encontra a próxima tabela que corresponde à máscara.

## Exemplo

```c
while (AdsFindNextTable(hConnect, hFind, szFile, &usLen) == AE_SUCCESS) {
    // Processar szFile
}
```

## Ver Também

- [AdsFindFirstTable]({{ site.baseurl }}/pt/funcoes/ads-find-first-table/)
- [AdsFindClose]({{ site.baseurl }}/pt/funcoes/ads-find-close/)
- [AdsCheckExistence]({{ site.baseurl }}/pt/funcoes/ads-check-existence/)

---

[AdsFindClose →]({{ site.baseurl }}/pt/funcoes/ads-find-close/)
