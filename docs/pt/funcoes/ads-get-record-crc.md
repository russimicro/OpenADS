---
title: AdsGetRecordCRC
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-record-crc/
---

# AdsGetRecordCRC

Retorna o CRC do registo atual.

## Sintaxe

```c
UNSIGNED32 AdsGetRecordCRC(ADSHANDLE hTable, UNSIGNED32* pulCRC,
                           UNSIGNED32 ulOption);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pulCRC` | `UNSIGNED32*` | Ponteiro para receber o CRC. |
| `ulOption` | `UNSIGNED32` | Opção (reservada). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_FUNCTION_NOT_AVAILABLE` para tabelas remotas.

## Descrição

`AdsGetRecordCRC` retorna o CRC-32 do conteúdo bruto do registo atual.

**Nota:** Não está disponível para tabelas remotas.

## Exemplo

```c
UNSIGNED32 ulCRC;
AdsGetRecordCRC(hTable, &ulCRC, 0);
// ulCRC contém o CRC do registo
```

## Ver Também

- [AdsGetRecord]({{ site.baseurl }}/pt/funcoes/ads-get-record/)
- [AdsGetRecordLength]({{ site.baseurl }}/pt/funcoes/ads-get-record-length/)
- [AdsGetRecordNum]({{ site.baseurl }}/pt/funcoes/ads-get-record-num/)

---

[AdsGetNumParams →]({{ site.baseurl }}/pt/funcoes/ads-get-num-params/)
