---
title: AdsSetBinary
layout: default
parent: Referência da API
nav_order: 36
permalink: /pt/funcoes/ads-set-binary/
---

# AdsSetBinary

Grava dados binários em um campo.

## Sintaxe

```c
UNSIGNED32 AdsSetBinary(ADSHANDLE hTable, UNSIGNED8* pucField,
                        UNSIGNED16 usBinaryType, UNSIGNED32 ulTotalBytes,
                        UNSIGNED32 ulOffset, UNSIGNED8* pucBuf,
                        UNSIGNED32 ulBytes);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucField` | `UNSIGNED8*` | Nome ou índice ordinal (1-based) do campo. |
| `usBinaryType` | `UNSIGNED16` | Tipo do dado binário (ADS_BINARY ou ADS_IMAGE). |
| `ulTotalBytes` | `UNSIGNED32` | Tamanho total dos dados binários. |
| `ulOffset` | `UNSIGNED32` | Deslocamento no buffer onde escrever. |
| `pucBuf` | `UNSIGNED8*` | Buffer com os dados binários. |
| `ulBytes` | `UNSIGNED32` | Número de bytes a escrever. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsSetBinary` grava dados binários em um campo BLOB (memo binário). A gravação pode ser feita em blocos, usando o deslocamento para especificar onde no campo os dados devem ser escritos. O tipo deve ser `ADS_BINARY` para dados binários gerais ou `ADS_IMAGE` para imagens.

## Exemplo

```c
UNSIGNED8 aucData[1024];
// ... preencher aucData ...
AdsSetBinary(hTable, "Foto", ADS_IMAGE, 1024, 0, aucData, 1024);
```

## Ver Também

- [AdsGetBinary]({{ site.baseurl }}/pt/funcoes/ads-get-binary/)
- [AdsGetBinaryLength]({{ site.baseurl }}/pt/funcoes/ads-get-binary-length/)
- [AdsBinaryToFile]({{ site.baseurl }}/pt/funcoes/ads-binary-to-file/)

---

[AdsSetJulian →]({{ site.baseurl }}/pt/funcoes/ads-set-julian/)
