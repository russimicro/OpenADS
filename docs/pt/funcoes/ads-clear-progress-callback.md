---
title: AdsClearProgressCallback
layout: default
parent: Referência da API
nav_order: 15
permalink: /pt/funcoes/ads-clear-progress-callback/
---

# AdsClearProgressCallback

Remove a função de callback de progresso.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsClearProgressCallback(void);
```

## Parâmetros

Nenhum.

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsClearProgressCallback` remove a função de callback de progresso previamente registrada com `AdsRegisterProgressCallback`.

## Exemplo

```c
AdsClearProgressCallback();
```

## Ver Também

- [AdsRegisterProgressCallback]({{ site.baseurl }}/pt/funcoes/ads-register-progress-callback/)

---

[AdsCloneTable →]({{ site.baseurl }}/pt/funcoes/ads-clone-table/)
