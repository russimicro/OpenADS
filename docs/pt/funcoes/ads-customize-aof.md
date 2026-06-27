---
title: AdsCustomizeAOF
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-customize-aof/
---

# AdsCustomizeAOF

Personaliza a AOF adicionando ou removendo registos específicos.

## Sintaxe

```c
UNSIGNED32 AdsCustomizeAOF(ADSHANDLE hTable, UNSIGNED32 ulNumRecords,
                           UNSIGNED32* pulRecords, UNSIGNED16 usOption);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hTable` | `ADSHANDLE` | Handle da tabela. |
| `ulNumRecords` | `UNSIGNED32` | Número de registos. |
| `pulRecords` | `UNSIGNED32*` | Array com os números dos registos. |
| `usOption` | `UNSIGNED16` | Opção: `ADS_AOF_ADD_RECORD` ou `ADS_AOF_REMOVE_RECORD`. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. `AE_FUNCTION_NOT_AVAILABLE` para tabelas remotas.

## Descrição

`AdsCustomizeAOF` permite personalizar a AOF adicionando ou removendo registos específicos da seleção.

**Nota:** Esta função não está disponível para tabelas remotas.

## Exemplo

```c
UNSIGNED32 records[] = {1, 3, 5, 7};
AdsCustomizeAOF(hTable, 4, records, ADS_AOF_ADD_RECORD);
```

## Ver Também

- [AdsSetAOF]({{ site.baseurl }}/pt/funcoes/ads-set-aof/)
- [AdsGetAOF]({{ site.baseurl }}/pt/funcoes/ads-get-aof/)
- [AdsIsRecordInAOF]({{ site.baseurl }}/pt/funcoes/ads-is-record-in-aof/)

---

[AdsIsRecordInAOF →]({{ site.baseurl }}/pt/funcoes/ads-is-record-in-aof/)
