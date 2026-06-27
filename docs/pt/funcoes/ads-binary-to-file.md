---
title: AdsBinaryToFile
layout: default
parent: Referência da API
nav_order: 6
permalink: /pt/funcoes/ads-binary-to-file/
---

# AdsBinaryToFile

Exporta o conteúdo de um campo binário para um arquivo.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsBinaryToFile(ADSHANDLE hTable,
                                      UNSIGNED8* pucField,
                                      UNSIGNED8* pucPath);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucField` | `UNSIGNED8*` | Nome do campo binário. |
| `pucPath` | `UNSIGNED8*` | Caminho do arquivo de saída. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsBinaryToFile` exporta o conteúdo completo de um campo binário (memo binário) para um arquivo no disco.

## Exemplo

```c
AdsBinaryToFile(hTable, "FOTO", "C:\\fotos\\foto1.jpg");
```

## Ver Também

- [AdsFileToBinary]({{ site.baseurl }}/pt/funcoes/ads-file-to-binary/)
- [AdsGetBinary]({{ site.baseurl }}/pt/funcoes/ads-get-binary/)
- [AdsSetBinary]({{ site.baseurl }}/pt/funcoes/ads-set-binary/)

---

[AdsCacheOpenCursors →]({{ site.baseurl }}/pt/funcoes/ads-cache-open-cursors/)
