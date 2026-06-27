---
title: AdsStudioStop
layout: default
parent: Referência da API
nav_order: 48
permalink: /pt/funcoes/ads-studio-stop/
---

# AdsStudioStop

Para o console web Studio do OpenADS.

## Sintaxe

```c
UNSIGNED32 AdsStudioStop(void);
```

## Parâmetros

Nenhum.

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsStudioStop` para o servidor HTTP do Studio e libera todos os recursos associados. Após essa chamada, o console web não está mais acessível.

## Exemplo

```c
AdsStudioStop();
// Studio parado, porta liberada
```

## Ver Também

- [AdsStudioStart]({{ site.baseurl }}/pt/funcoes/ads-studio-start/)
- [AdsStudioPort]({{ site.baseurl }}/pt/funcoes/ads-studio-port/)

---

[Início]({{ site.baseurl }}/pt/funcoes/)
