---
title: AdsStudioStart
layout: default
parent: Referência da API
nav_order: 47
permalink: /pt/funcoes/ads-studio-start/
---

# AdsStudioStart

Inicia o console web Studio do OpenADS.

## Sintaxe

```c
UNSIGNED32 AdsStudioStart(UNSIGNED16 usPort, UNSIGNED8* pucDataDir);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `usPort` | `UNSIGNED16` | Porta TCP para o servidor HTTP (0 = efêmera). |
| `pucDataDir` | `UNSIGNED8*` | Diretório de dados (NULL = diretório atual). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsStudioStart` inicia o console web Studio do OpenADS, que fornece uma interface de administração via navegador. O Studio abre tabelas read-only usando conexões ABI de curta duração.

**Variáveis de ambiente:**
- `OPENADS_STUDIO_PORT` — Porta para auto-inicialização quando a DLL é carregada.
- `OPENADS_STUDIO_DATA` — Diretório de dados para auto-inicialização.
- `OPENADS_STUDIO_HOST` — Endereço de vinculação (padrão: 127.0.0.1).

**Nota:** O alvo Studio só é compilado quando OpenADS é construído com `-DOPENADS_WITH_HTTP=ON`. Sem essa flag, a função retorna `AE_FUNCTION_NOT_AVAILABLE`.

## Exemplo

```c
UNSIGNED32 ulResult = AdsStudioStart(8080, "C:\\MeusDados");
if (ulResult == AE_SUCCESS) {
    // Studio rodando na porta 8080
}
```

## Ver Também

- [AdsStudioStop]({{ site.baseurl }}/pt/funcoes/ads-studio-stop/)
- [AdsStudioPort]({{ site.baseurl }}/pt/funcoes/ads-studio-port/)

---

[AdsStudioStop →]({{ site.baseurl }}/pt/funcoes/ads-studio-stop/)
