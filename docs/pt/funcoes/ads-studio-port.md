---
title: AdsStudioPort
layout: default
parent: Referência da API
nav_order: 46
permalink: /pt/funcoes/ads-studio-port/
---

# AdsStudioPort

Retorna a porta na qual o Studio está em execução.

## Sintaxe

```c
UNSIGNED32 AdsStudioPort(UNSIGNED16* pusPort);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `pusPort` | `UNSIGNED16*` | Ponteiro para variável que recebe o número da porta. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsStudioPort` retorna a porta TCP na qual o Studio web está atualmente vinculado. Retorna 0 quando o Studio não está em execução. É útil quando `AdsStudioStart` foi chamado com porta 0 (efêmera) e o chamador precisa saber qual porta o sistema operacional selecionou.

## Exemplo

```c
UNSIGNED16 usPort;
AdsStudioPort(&usPort);
if (usPort > 0) {
    printf("Studio rodando na porta %d\n", usPort);
}
```

## Ver Também

- [AdsStudioStart]({{ site.baseurl }}/pt/funcoes/ads-studio-start/)
- [AdsStudioStop]({{ site.baseurl }}/pt/funcoes/ads-studio-stop/)

---

[AdsStudioStart →]({{ site.baseurl }}/pt/funcoes/ads-studio-start/)
