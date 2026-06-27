---
title: AdsFileToBinary
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-file-to-binary/
---

# AdsFileToBinary

Importa um arquivo para um campo binário.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsFileToBinary(ADSHANDLE hTable, UNSIGNED8* pucField, UNSIGNED16 usType, UNSIGNED8* pucPath);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucField` | `UNSIGNED8*` | Nome do campo binário/blob. |
| `usType` | `UNSIGNED16` | Tipo do binário (ADS_BINARY ou ADS_IMAGE). |
| `pucPath` | `UNSIGNED8*` | Caminho do arquivo a ser importado. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsFileToBinary` importa o conteúdo de um arquivo para um campo binário ou de imagem na tabela. O arquivo é lido e seu conteúdo é armazenado no campo especificado.

## Exemplo

```c
AdsFileToBinary(hTable, "Foto", ADS_IMAGE, "C:\\fotos\\cliente.jpg");
```

## Ver Também

- [AdsBinaryToFile]({{ site.baseurl }}/pt/funcoes/ads-binary-to-file/)
- [AdsGetBinary]({{ site.baseurl }}/pt/funcoes/ads-get-binary/)
- [AdsGetBinaryLength]({{ site.baseurl }}/pt/funcoes/ads-get-binary-length/)

---

[AdsFindFirstTable62 →]({{ site.baseurl }}/pt/funcoes/ads-find-first-table-62/)
