---
title: AdsRegisterProgressCallback
layout: default
parent: Referência da API
nav_order: 32
permalink: /pt/funcoes/ads-register-progress-callback/
---

# AdsRegisterProgressCallback

Registra uma função de callback de progresso para operações longas.

## Sintaxe

```c
UNSIGNED32 AdsRegisterProgressCallback(void* pCallback);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `pCallback` | `void*` | Ponteiro para a função de callback de progresso. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsRegisterProgressCallback` registra uma função de callback que será chamada periodicamente durante operações longas (como reindexação ou busca FTS) para reportar o progresso. Para remover a função registrada, use `AdsClearProgressCallback`.

## Exemplo

```c
void MeuProgresso(UNSIGNED16 usPercentual) {
    printf("Progresso: %d%%\n", usPercentual);
}
AdsRegisterProgressCallback(MeuProgresso);
```

## Ver Também

- [AdsClearProgressCallback]({{ site.baseurl }}/pt/funcoes/ads-clear-progress-callback/)
- [AdsRegisterCallbackFunction]({{ site.baseurl }}/pt/funcoes/ads-register-callback-function/)
- [AdsReindex]({{ site.baseurl }}/pt/funcoes/ads-reindex/)

---

[AdsReindex61 →]({{ site.baseurl }}/pt/funcoes/ads-reindex-61/)
