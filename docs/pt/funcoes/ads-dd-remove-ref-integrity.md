---
title: AdsDDRemoveRefIntegrity
layout: default
parent: Referência da API
nav_order: 1
permalink: /pt/funcoes/ads-dd-remove-ref-integrity/
---

# AdsDDRemoveRefIntegrity

Remove uma regra de integridade referencial do dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDRemoveRefIntegrity(ADSHANDLE hConnect, UNSIGNED8* pucName);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `hConnect` | `ADSHANDLE` | Handle da conexão com o dicionário de dados. |
| `pucName` | `UNSIGNED8*` | Nome da regra de integridade referencial. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDRemoveRefIntegrity` remove uma regra de integridade referencial (RI) existente do dicionário de dados. Após a remoção, as restrições de integridade entre as tabelas pai e filha não serão mais aplicadas.

## Exemplo

```c
AdsDDRemoveRefIntegrity(hConnect, "RIClientesPedidos");
```

## Ver Também

- [AdsDDCreateRefIntegrity]({{ site.baseurl }}/pt/funcoes/ads-dd-create-ref-integrity/)
- [AdsDDGetRefIntegrityProperty]({{ site.baseurl }}/pt/funcoes/ads-dd-get-ref-integrity-property/)

---

[AdsDDRemoveTable →]({{ site.baseurl }}/pt/funcoes/ads-dd-remove-table/)
