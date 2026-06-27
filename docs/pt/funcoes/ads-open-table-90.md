---
title: AdsOpenTable90
layout: default
parent: Referência da API
nav_order: 29
permalink: /pt/funcoes/ads-open-table-90/
---

# AdsOpenTable90

Abre uma tabela com suporte a colação (versão 90).

## Sintaxe

```c
UNSIGNED32 AdsOpenTable90(ADSHANDLE hConnect, UNSIGNED8* pucName,
                          UNSIGNED8* pucAlias, UNSIGNED16 usTableType,
                          UNSIGNED16 usCharType, UNSIGNED16 usLockType,
                          UNSIGNED16 usCheckRights, UNSIGNED32 ulOptions,
                          UNSIGNED8* pucCollation, ADSHANDLE* phTable);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão (0 para local). |
| `pucName` | `UNSIGNED8*` | Nome ou caminho do arquivo da tabela. |
| `pucAlias` | `UNSIGNED8*` | Alias (nome alternativo) da tabela. |
| `usTableType` | `UNSIGNED16` | Tipo da tabela (ADS_CDX, ADS_ADT, ADS_VFP, etc.). |
| `usCharType` | `UNSIGNED16` | Tipo de caracteres (ADS_ANSI ou ADS_OEM). |
| `usLockType` | `UNSIGNED16` | Tipo de bloqueio (ADS_SHARED, ADS_EXCLUSIVE, etc.). |
| `usCheckRights` | `UNSIGNED16` | Verificar direitos de acesso (ADS_CHECKRIGHTS ou 0). |
| `ulOptions` | `UNSIGNED32` | Opções adicionais (ex: ADS_COMPRESS_ALWAYS). |
| `pucCollation` | `UNSIGNED8*` | Nome da colação (NULL para padrão). |
| `phTable` | `ADSHANDLE*` | Ponteiro para handle de saída da tabela. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsOpenTable90` abre uma tabela Advantage com suporte a colação especificada. Essa variante (versão 90) permite especificar a colação ao abrir a tabela, o que afeta a ordenação e comparação de strings.

## Exemplo

```c
ADSHANDLE hTable;
AdsOpenTable90(hConnect, "Clientes", "CLI", ADS_CDX, ADS_ANSI,
               ADS_SHARED, ADS_CHECKRIGHTS, 0, "LATIN1", &hTable);
```

## Ver Também

- [AdsOpenTable]({{ site.baseurl }}/pt/funcoes/ads-open-table/)
- [AdsCreateTable90]({{ site.baseurl }}/pt/funcoes/ads-create-table-90/)
- [AdsCloseTable]({{ site.baseurl }}/pt/funcoes/ads-close-table/)

---

[AdsRefreshRecord →]({{ site.baseurl }}/pt/funcoes/ads-refresh-record/)
