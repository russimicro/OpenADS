---
title: AdsGetBinaryLength
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-binary-length/
---

# AdsGetBinaryLength

Obtém o comprimento dos dados binários de um campo blob.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsGetBinaryLength(ADSHANDLE hTable, UNSIGNED8* pucField, UNSIGNED32* pulLength);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucField` | `UNSIGNED8*` | Nome do campo binário/blob. |
| `pulLength` | `UNSIGNED32*` | Ponteiro para receber o comprimento em bytes. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsGetBinaryLength` retorna o comprimento total dos dados binários armazenados em um campo blob. Esta função é útil para alocar buffers do tamanho correto antes de chamar AdsGetBinary.

## Exemplo

```c
UNSIGNED32 ulLength;

AdsGetBinaryLength(hTable, "Foto", &ulLength);
if (ulLength > 0) {
    UNSIGNED8* pBuffer = malloc(ulLength);
    AdsGetBinary(hTable, "Foto", 0, pBuffer, &ulLength);
    free(pBuffer);
}
```

## Ver Também

- [AdsGetBinary]({{ site.baseurl }}/pt/funcoes/ads-get-binary/)
- [AdsFileToBinary]({{ site.baseurl }}/pt/funcoes/ads-file-to-binary/)
- [AdsBinaryToFile]({{ site.baseurl }}/pt/funcoes/ads-binary-to-file/)

---

[AdsGetBinary →]({{ site.baseurl }}/pt/funcoes/ads-get-binary/)
