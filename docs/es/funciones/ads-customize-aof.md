---
title: AdsCustomizeAOF
layout: default
parent: Referencia API
nav_order: 1
permalink: /es/funciones/ads-customize-aof/
---

# AdsCustomizeAOF

Fuerza registros individuales dentro o fuera del Advantage Optimized Filter activo.

## Sintaxis

```c
UNSIGNED32 AdsCustomizeAOF(ADSHANDLE hTable, UNSIGNED32 ulNumRecords,
                           UNSIGNED32 *pulRecords, UNSIGNED16 usOption);
```

## Parámetros

| Parámetro | Tipo | Descripción |
|-----------|------|-------------|
| `hTable` | `ADSHANDLE` | Handle de la tabla con un AOF activo. |
| `ulNumRecords` | `UNSIGNED32` | Número de registros en `pulRecords`. |
| `pulRecords` | `UNSIGNED32*` | Array de números de registro a añadir o quitar. |
| `usOption` | `UNSIGNED16` | `ADS_AOF_ADD_RECORD` (1) para incluir, `ADS_AOF_REMOVE_RECORD` (2) para excluir. |

## Valor de Retorno

`AE_SUCCESS` (0) en caso de éxito. `AE_INTERNAL_ERROR` (5000) si no hay AOF activo, la opción es inválida o el handle es desconocido. `AE_FUNCTION_NOT_AVAILABLE` (5004) para tablas remotas.

## Descripción

`AdsCustomizeAOF` anula manualmente la pertenencia de registros concretos al conjunto resultado del AOF instalado con `AdsSetAOF`. `ADS_AOF_ADD_RECORD` hace visibles los registros listados aunque no cumplan la expresión de filtro; `ADS_AOF_REMOVE_RECORD` los oculta aunque sí la cumplan.

El cambio actualiza el bitmap AOF retenido y lo reinstala, de modo que la navegación posterior (`AdsGotoTop`, `AdsGotoBottom`, `AdsSkip`) refleja de inmediato el conjunto personalizado. Los números de registro fuera del rango de la tabla se ignoran. La personalización se descarta al limpiar el AOF con `AdsClearAOF` o al reemplazarlo con un nuevo `AdsSetAOF`.

Requiere un AOF activo y no está disponible para tablas remotas.

## Ejemplo

```c
AdsSetAOF(hTable, "BALANCE > 0", 0);

// Incluir siempre el registro 4, aunque su saldo sea 0.
UNSIGNED32 add[] = { 4 };
AdsCustomizeAOF(hTable, 1, add, ADS_AOF_ADD_RECORD);

// Ocultar el registro 2 sin importar su saldo.
UNSIGNED32 rem[] = { 2 };
AdsCustomizeAOF(hTable, 1, rem, ADS_AOF_REMOVE_RECORD);
```

## Ver También

- [AdsSetAOF]({{ site.baseurl }}/es/funciones/ads-set-aof/)
- [AdsGetAOF]({{ site.baseurl }}/es/funciones/ads-get-aof/)
- [AdsClearAOF]({{ site.baseurl }}/es/funciones/ads-clear-aof/)

---

[AdsGetAOF →]({{ site.baseurl }}/es/funciones/ads-get-aof/)
