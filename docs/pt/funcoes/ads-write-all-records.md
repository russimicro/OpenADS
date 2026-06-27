---
title: AdsWriteAllRecords
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-write-all-records/
---

# AdsWriteAllRecords

Grava todos os registos pendentes.

## Sintaxe

```c
UNSIGNED32 AdsWriteAllRecords(void);
```

## Parâmetros

Nenhum.

## Valor de Retorno

`AE_SUCCESS` (0) sempre.

## Descrição

`AdsWriteAllRecords` grava todos os registos pendentes em todas as tabelas abertas. No OpenADS, retorna sucesso sem operação adicional (o flush é feito por tabela).

## Exemplo

```c
AdsWriteAllRecords();
```

## Ver Também

- [AdsWriteRecord]({{ site.baseurl }}/pt/funcoes/ads-write-record/)
- [AdsFlushFileBuffers]({{ site.baseurl }}/pt/funcoes/ads-flush-file-buffers/)
- [AdsCloseTable]({{ site.baseurl }}/pt/funcoes/ads-close-table/)

---

[AdsApplicationExit →]({{ site.baseurl }}/pt/funcoes/ads-application-exit/)
