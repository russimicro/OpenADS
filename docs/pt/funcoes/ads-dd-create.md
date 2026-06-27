---
title: AdsDDCreate
layout: default
parent: Referência da API
nav_order: 32
permalink: /pt/funcoes/ads-dd-create/
---

# AdsDDCreate

Cria um novo dicionário de dados.

## Sintaxe

```c
UNSIGNED32 ENTRYPOINT AdsDDCreate(UNSIGNED8*  pucDictionary,
                                  UNSIGNED16  bEncrypt,
                                  UNSIGNED8*  pucAdminPassword,
                                  ADSHANDLE*  phConnect);
```

## Parâmetros

| Parâmetro | Tipo | Descrição |
|-----------|------|-----------|
| `pucDictionary` | `UNSIGNED8*` | Caminho do dicionário de dados. |
| `bEncrypt` | `UNSIGNED16` | Criptografar o dicionário (1=sim, 0=não). |
| `pucAdminPassword` | `UNSIGNED8*` | Palavra-passe do administrador. |
| `phConnect` | `ADSHANDLE*` | Ponteiro para receber o handle da conexão. |

## Valor de Retorno

`AE_SUCCESS` (0) em caso de sucesso. Código de erro se falhar.

## Descrição

`AdsDDCreate` cria um novo dicionário de dados vazio. O dicionário pode ser protegido com palavra-passe e criptografia.

## Exemplo

```c
ADSHANDLE hConnect;
AdsDDCreate("C:\\dados\\meu_dd.add", 1, "admin123", &hConnect);
```

## Ver Também

- [AdsDDAddTable]({{ site.baseurl }}/pt/funcoes/ads-dd-add-table/)
- [AdsDDCreateUser]({{ site.baseurl }}/pt/funcoes/ads-dd-create-user/)

---

[AdsDDCreateFunction →]({{ site.baseurl }}/pt/funcoes/ads-dd-create-function/)
