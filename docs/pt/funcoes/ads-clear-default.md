---
title: AdsClearDefault
layout: default
parent: Referência da API
nav_order: 14
permalink: /pt/funcoes/ads-clear-default/
---

# AdsClearDefault

Limpa o diretório padrão.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsClearDefault(void);
```

## Parâmetros

Nenhum.

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsClearDefault` remove o diretório padrão previamente definido com `AdsSetDefault`. Após esta chamada, as operações usarão o diretório de trabalho atual.

## Exemplo

```c
AdsClearDefault();
```

## Ver Também

- [AdsSetDefault]({{ site.baseurl }}/pt/funcoes/ads-set-default/)
- [AdsGetDefault]({{ site.baseurl }}/pt/funcoes/ads-get-default/)

---

[AdsClearProgressCallback →]({{ site.baseurl }}/pt/funcoes/ads-clear-progress-callback/)
