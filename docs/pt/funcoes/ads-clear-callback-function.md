---
title: AdsClearCallbackFunction
layout: default
parent: Referência da API
nav_order: 13
permalink: /pt/funcoes/ads-clear-callback-function/
---

# AdsClearCallbackFunction

Remove a função de callback registrada.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsClearCallbackFunction(void);
```

## Parâmetros

Nenhum.

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsClearCallbackFunction` remove a função de callback previamente registrada com `AdsRegisterCallbackFunction`. Após esta chamada, nenhuma função de callback será invocada.

## Exemplo

```c
AdsClearCallbackFunction();
```

## Ver Também

- [AdsRegisterCallbackFunction]({{ site.baseurl }}/pt/funcoes/ads-register-callback-function/)

---

[AdsClearDefault →]({{ site.baseurl }}/pt/funcoes/ads-clear-default/)
