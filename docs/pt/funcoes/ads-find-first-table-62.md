---
title: AdsFindFirstTable62
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-find-first-table-62/
---

# AdsFindFirstTable62

Encontra a primeira tabela que corresponde a uma máscara.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsFindFirstTable62(ADSHANDLE hConnect, UNSIGNED8* pucFileMask, UNSIGNED8* pucFirstDD, UNSIGNED16* pusDDLen, UNSIGNED8* pucFirstFile, UNSIGNED16* pusFileLen, ADSHANDLE* phFind);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `pucFileMask` | `UNSIGNED8*` | Máscara de busca (ex: "*.dbf"). |
| `pucFirstDD` | `UNSIGNED8*` | Buffer para o primeiro dicionário de dados. |
| `pusDDLen` | `UNSIGNED16*` | Comprimento do buffer do DD. |
| `pucFirstFile` | `UNSIGNED8*` | Buffer para o primeiro nome de arquivo. |
| `pusFileLen` | `UNSIGNED16*` | Comprimento do buffer do arquivo. |
| `phFind` | `ADSHANDLE*` | Ponteiro para receber o handle de busca. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsFindFirstTable62` inicia a busca por tabelas que correspondem à máscara especificada. O handle de busca retornado deve ser usado com AdsFindNextTable62 para continuar a busca e AdsFindClose para liberar os recursos.

## Exemplo

```c
ADSHANDLE hFind;
UNSIGNED16 usDDLen = 256, usFileLen = 256;
UNSIGNED8 aucDD[256], aucFile[256];

AdsFindFirstTable62(hConnect, "*.dbf", aucDD, &usDDLen, aucFile, &usFileLen, &hFind);
// Processar primeira tabela...
while (AdsFindNextTable62(hConnect, hFind, aucDD, &usDDLen, aucFile, &usFileLen) == AE_SUCCESS) {
    // Processar próximas tabelas
}
AdsFindClose(hConnect, hFind);
```

## Ver Também

- [AdsFindNextTable62]({{ site.baseurl }}/pt/funcoes/ads-find-next-table-62/)
- [AdsFindClose]({{ site.baseurl }}/pt/funcoes/ads-find-close/)

---

[AdsFindNextTable62 →]({{ site.baseurl }}/pt/funcoes/ads-find-next-table-62/)
