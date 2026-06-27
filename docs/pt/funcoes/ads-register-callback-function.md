---
title: AdsRegisterCallbackFunction
layout: default
parent: Referência da API
nav_order: 31
permalink: /pt/funcoes/ads-register-callback-function/
---

# AdsRegisterCallbackFunction

Registra uma função de callback para notificações do servidor.

## Sintaxe

```c
UNSIGNED32 AdsRegisterCallbackFunction(void* pCallback);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `pCallback` | `void*` | Ponteiro para a função de callback. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsRegisterCallbackFunction` registra uma função de callback que será chamada pelo servidor para notificar eventos. Para remover a função registrada, use `AdsClearCallbackFunction`.

## Exemplo

```c
void MeuCallback(ADSHANDLE hConnect, UNSIGNED16 usEvent, void* pData) {
    // Lida com o evento
}
AdsRegisterCallbackFunction(MeuCallback);
```

## Ver Também

- [AdsClearCallbackFunction]({{ site.baseurl }}/pt/funcoes/ads-clear-callback-function/)
- [AdsRegisterProgressCallback]({{ site.baseurl }}/pt/funcoes/ads-register-progress-callback/)
- [AdsClearProgressCallback]({{ site.baseurl }}/pt/funcoes/ads-clear-progress-callback/)

---

[AdsRegisterProgressCallback →]({{ site.baseurl }}/pt/funcoes/ads-register-progress-callback/)
