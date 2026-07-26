# VALIDACION — OpenADS respeta los indices originales (SELECT-SQL + indices temporales)

> Cierre 2026-06-26. Documento de evidencia. **PREMISA SUPERADA.**
> Continuacion de `PROMPT_OPENADS_RESPETAR_INDICES_ORIGINALES.md`.

## Premisa (requisito del usuario)

> *"El OpenADS debe RESPETAR los indices originales. Un SELECT-SQL + indices
> temporales con un tag o dos tags NO debe afectar al index original. PARA NADA
> debe ajustar el ERP, dado que asi funciona con los drivers ADS-SAP y DBFCDX."*

**Criterio de aceptacion (invariante, testeable):** para CUALQUIER SELECT seguido
de los `INDEX ON ... TO <tmp>` del ERP, el hash/byte-content y el mtime de los
`.cdx` OFICIALES de las tablas fuente quedan **identicos** antes y despues.

---

## Veredicto: APROBADO (invariante cumplido)

El flujo real del ERP se reprodujo de punta a punta y OpenADS **no escribio ni
un byte** de los `.cdx` oficiales. Los indices temporales viven en su bag aparte,
exactamente como ADS-SAP y DBFCDX. **No hay bug de corrupcion.** Cero cambios al ERP.

---

## Evidencia 1 — Diagnostico por instrumentacion (`cdxwrite.log`)

Se instrumento `src/drivers/cdx/cdx_index.cpp` para loguear cada `write_at` fisico
a un `.cdx` (path + offset + caller: `flush_page_` / `set_options` /
`rewrite_header_` / `free_tree_`) y cada apertura (`OPEN`), a
`C:\OpenADS\cdxwrite.log` (activa por defecto durante el diagnostico; revertir a
opt-in con `OPENADS_CDX_WRITE_LOG=1` antes de produccion).

Sesion real del ERP (empresa 00, `F:\ZERUS64\00\BASES\`), busqueda de articulo por
nombre — flujo `buscaEnNombre()` (`FUNCSCLI.PRG:7819`) que corre:
```
ADSRunSQL("Select * From [articulo.dat] as a WHERE ... ORDER BY a.cnombreart", ...)
INDEX ON cNombreArt TAG ORDER_NOMBRE TO &(cPatRed+"\TEMP\"+FileInI)
```

Resultado del log:
- El ERP **creo los indices temporales** `f:\TEMP\REF_NOM000018254.CDX` y
  `f:\TEMP\REF_NOM000019312.CDX` (12:07:08 y 12:07:13).
- OpenADS **escribio SOLO en esos TEMP** (`rewrite_header_` + `flush_page_`).
- **Cero escrituras** a `ARTICULO.CDX` (ni `flush_page_`, ni `set_options`, ni
  `rewrite_header_`, ni `free_tree_`) durante toda la sesion.
- El oficial solo aparece como `OPEN` (lectura), nunca como destino de escritura.

## Evidencia 2 — Hash + mtime antes vs despues (la prueba del invariante)

Captura ANTES (12:12:41), busqueda SQL ejecutada por el usuario, captura DESPUES:

| CDX oficial          | md5 ANTES            | md5 DESPUES          | mtime        | Resultado  |
|----------------------|----------------------|----------------------|--------------|------------|
| ARTICULO.CDX         | 2ba232a09c6321dc     | 2ba232a09c6321dc     | 11:01:20     | OK intacto |
| CLIENTES.CDX         | ce3c071aba1b0054     | ce3c071aba1b0054     | 11:01:03     | OK intacto |
| CLASES.CDX           | 9d22870467588cf3     | 9d22870467588cf3     | 11:01:04     | OK intacto |
| BODEGAS.CDX          | 7f0c4845d5ed1622     | 7f0c4845d5ed1622     | 11:01:05     | OK intacto |

Los 4 indices oficiales quedaron **byte-identical** (mismo md5, mismo mtime,
mismo size). El invariante se cumple.

---

## Por que no hay bug (mecanismo)

1. **Guard de dirty en `CdxIndex::flush()`** (`cdx_index.cpp:1326`): si ninguna
   pagina del bag esta dirty, retorna sin escribir. Una apertura/navegacion de
   solo lectura no marca dirty -> no escribe -> el `.cdx` oficial no cambia.
2. **Materializacion del cursor SQL single-table** (`ace_exports.cpp:17415`): el
   SELECT con ORDER BY/DISTINCT/LIMIT se materializa a una tabla temporal
   (`_srt_*.dbf`) y se cierra la fuente, de modo que el `INDEX ON ... TO tmpbag`
   del ERP toca solo la temporal, no la tabla fuente ni su oficial.
3. **`INDEX ON ... TO <bag>` es estricto** (`AdsCreateIndex61`): el nuevo tag va
   solo al bag `<tmp>` (aqui `REF_NOM*`), nunca al `.cdx` estructural.

## Fix defensivo aplicado esta sesion (no corrige un bug activo; refuerza)

`Table::flush()` (`src/engine/table.cpp:1178`): no recorre/reescribe los indices
(`order_` + `extra_index_views_`) si la tabla se abrio read-only
(`mode_ == OpenMode::Read`). Mismo patron que ya usaban `lock_record_excl` /
`lock_table_excl`. Asi, aunque el cursor SQL alguna vez quedara abierto vivo en
modo Read, su cierre (`AdsCloseTable` -> `flush`) jamas tocara el CDX de
produccion. Tests unitarios: **721/721 OK**.

## Deploy

`openace64.dll` (con fix defensivo + instrumentacion de diagnostico) desplegada
en `F:\zerus64` y `C:\OpenADS`. Backups:
- `_BACKUP_RUSOFT_respeta_indices_20260626_114335/` (los 3 .cpp antes de tocar).
- `openace64.dll.bak_pre_cdxlog_20260626` junto a cada DLL desplegada.

## Pendiente (no bloquea la premisa)

- **Revertir la instrumentacion a opt-in** (`OPENADS_CDX_WRITE_LOG=1`) y borrar
  `C:\OpenADS\cdxwrite.log` antes de produccion (evita overhead/disco en cada
  corrida). Es un edit chico en `cdxw_log_path()`.

## Archivos / lineas clave

- `src/drivers/cdx/cdx_index.cpp`: `flush()` :1326 (guard dirty), `flush_page_`
  :423, instrumentacion `cdxw_*` (~:20-95, :512 OPEN log), `set_options` :1440,
  `rewrite_header_` :1539, `free_tree_` link :1464.
- `src/engine/table.cpp`: `flush()` :1178 (guard Read), `extra_index_views_`.
- `src/abi/ace_exports.cpp`: materializacion single-table :17415,
  `AdsCreateIndex61` :6685, M-AOF.6 production-CDX auto-open :3245,
  `AdsCloseTable` :4230 (flush al cierre).
- ERP (referencia, NO tocar): `FUNCSCLI.PRG:7819` `buscaEnNombre`.

## Build / deploy

- DLL: `cmake --build C:\OpenADS\OpenADS-main\build\default --config Release
  --target openads_ace --parallel`
- Tests: `--target openads_unit_tests` -> `build\default\tests\Release\openads_unit_tests.exe`
- Deploy: copiar `build\default\src\Release\openace64.dll` a `F:\zerus64` y
  `C:\OpenADS` (zeruswin CERRADO).
