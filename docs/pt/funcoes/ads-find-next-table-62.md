---
title: AdsFindNextTable62
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-find-next-table-62/
---

# AdsFindNextTable62

Encontra a próxima tabela que corresponde a uma máscara.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsFindNextTable62(ADSHANDLE hConnect, ADSHANDLE hFind, UNSIGNED8* pucDDName, UNSIGNED16* pusDDLen, UNSIGNED8* pucFileName, UNSIGNED16* pusFileLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão. |
| `hFind` | `ADSHANDLE` | Handle de busca retornado por AdsFindFirstTable62. |
| `pucDDName` | `UNSIGNED8*` | Buffer para o nome do dicionário de dados. |
| `pusDDLen` | `UNSIGNED16*` | Comprimento do buffer do DD. |
| `pucFileName` | `UNSIGNED8*` | Buffer para o nome do arquivo. |
| `pusFileLen` | `UNSIGNED16*` | Comprimento do buffer do arquivo. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsFindNextTable62` localiza a próxima tabela que corresponde aos critérios de busca iniciados por AdsFindFirstTable62. O handle de busca deve ser válido e ter sido retornado por uma chamada anterior.

## Exemplo

```c
while (AdsFindNextTable62(hConnect, hFind, aucDD, &usDDLen, aucFile, &usFileLen) == AE_SUCCESS) {
    printf("Tabela encontrada: %s\\n", aucFile);
}
```

## Ver Também

- [AdsFindFirstTable62]({{ site.baseurl }}/pt/funcoes/ads-find-first-table-62/)
- [AdsFindClose]({{ site.baseurl }}/pt/funcoes/ads-find-close/)

---

[AdsFTSSearch →]({{ site.baseurl }}/pt/funcoes/ads-fts-search/)
