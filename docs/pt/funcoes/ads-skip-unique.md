---
title: AdsSkipUnique
layout: default
parent: Referência da API
nav_order: 40
permalink: /pt/funcoes/ads-skip-unique/
---

# AdsSkipUnique

Pula para o próximo registro com chave única no índice.

## Sintaxe

```c
UNSIGNED32 AdsSkipUnique(ADSHANDLE hIndex, SIGNED32 lDirection);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hIndex` | `ADSHANDLE` | Handle do índice ativo. |
| `lDirection` | `SIGNED32` | Direção do salto (1=próximo, -1=anterior). |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsSkipUnique` navega para o próximo registro que tem um valor único no campo do índice. Isso é útil para iterar apenas sobre valores distintos, ignorando registros duplicados.

## Exemplo

```c
AdsSetIndexOrder(hTable, "Cidade");
AdsGotoTop(hTable);
while (!bEOF) {
    // Processar registro com cidade única
    AdsSkipUnique(hIndex, 1);
}
```

## Ver Também

- [AdsSkip]({{ site.baseurl }}/pt/funcoes/ads-skip/)
- [AdsSeek]({{ site.baseurl }}/pt/funcoes/ads-seek/)
- [AdsGotoTop]({{ site.baseurl }}/pt/funcoes/ads-goto-top/)

---

[AdsStmtClearTablePasswords →]({{ site.baseurl }}/pt/funcoes/ads-stmt-clear-table-passwords/)
