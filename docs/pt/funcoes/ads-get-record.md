---
title: AdsGetRecord
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-get-record/
---

# AdsGetRecord

Retorna o conteúdo bruto do registo atual.

## Sintaxe

```c
UNSIGNED32 AdsGetRecord(ADSHANDLE hTable, UNSIGNED8* pucRecord,
                        UNSIGNED32* pulLen);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `pucRecord` | `UNSIGNED8*` | Buffer para receber o registo. Se nulo, retorna o comprimento. |
| `pulLen` | `UNSIGNED32*` | Ponteiro para o tamanho do buffer. Na saída, contém o comprimento do registo. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_NO_CURRENT_RECORD` se não houver registo atual.

## Descrição

`AdsGetRecord` retorna o conteúdo bruto (binário) do registo atual. Se o buffer for nulo ou o comprimento for 0, a função retorna o comprimento necessário para alocar o buffer.

**Nota:** Esta função não está disponível para tabelas remotas.

## Exemplo

```c
// Primeiro: obter o comprimento
UNSIGNED32 ulLen = 0;
AdsGetRecord(hTable, nullptr, &ulLen);

// Segundo: alocar e obter o registo
std::vector<UNSIGNED8> buf(ulLen);
AdsGetRecord(hTable, buf.data(), &ulLen);
```

## Ver Também

- [AdsSetRecord]({{ site.baseurl }}/pt/funcoes/ads-set-record/)
- [AdsGetRecordLength]({{ site.baseurl }}/pt/funcoes/ads-get-record-length/)
- [AdsGetField]({{ site.baseurl }}/pt/funcoes/ads-get-field/)

---

[AdsSetRecord →]({{ site.baseurl }}/pt/funcoes/ads-set-record/)
