---
title: AdsGetBinary
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-binary/
---

# AdsGetBinary

Obtém dados binários de um campo blob.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsGetBinary(ADSHANDLE hTable, UNSIGNED8* pucField, UNSIGNED32 ulOffset, UNSIGNED8* pucBuf, UNSIGNED32* pulLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucField` | `UNSIGNED8*` | Nome do campo binário/blob. |
| `ulOffset` | `UNSIGNED32` | Offset em bytes a partir do início dos dados. |
| `pucBuf` | `UNSIGNED8*` | Buffer para receber os dados. |
| `pulLen` | `UNSIGNED32*` | Comprimento do buffer; retorna os bytes lidos. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsGetBinary` recupera dados binários de um campo blob, permitindo ler porções específicas usando offset. Isso é útil para campos binários grandes onde não se deseja ler todos os dados de uma vez.

## Exemplo

```c
UNSIGNED32 ulLen = 1024;
UNSIGNED8 aucBuffer[1024];

AdsGetBinary(hTable, "Anexo", 0, aucBuffer, &ulLen);
```

## Ver Também

- [AdsGetBinaryLength]({{ site.baseurl }}/pt/funcoes/ads-get-binary-length/)
- [AdsSetBinary]({{ site.baseurl }}/pt/funcoes/ads-set-binary/)
- [AdsFileToBinary]({{ site.baseurl }}/pt/funcoes/ads-file-to-binary/)

---

[AdsGetBinaryLength →]({{ site.baseurl }}/pt/funcoes/ads-get-binary-length/)
