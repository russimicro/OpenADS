---
title: AdsDDFindClose
layout: default
parent: Referência da API
nav_order: 47
permalink: /pt/funcoes/ads-dd-find-close/
---

# AdsDDFindClose

Fecha uma operação de busca no dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDFindClose(ADSHANDLE  hObject,
                                     ADSHANDLE  hFindHandle);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hObject` | `ADSHANDLE` | Handle do dicionário de dados. |
| `hFindHandle` | `ADSHANDLE` | Handle da busca. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDFindClose` fecha uma operação de busca iniciada por `AdsDDFindFirstObject` ou `AdsDDFindNextObject`, liberando os recursos associados.

## Exemplo

```c
AdsDDFindClose(hDD, hFind);
```

## Ver Também

- [AdsDDFindFirstObject]({{ site.baseurl }}/pt/funcoes/ads-dd-find-first-object/)
- [AdsDDFindNextObject]({{ site.baseurl }}/pt/funcoes/ads-dd-find-next-object/)

---

[AdsDDFindFirstObject →]({{ site.baseurl }}/pt/funcoes/ads-dd-find-first-object/)
