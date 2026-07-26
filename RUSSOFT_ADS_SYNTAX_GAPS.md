# OpenADS — Sintaxis ADS no reconocida (caso ERP Russoft/Zerus)

Contexto: ERP Harbour + FiveWin que corría sobre Advantage Client Engine
(`ace64.dll`) y ahora enlaza `openace64.lib` (OpenADS) en el build MSVC x64.
Todas las consultas listadas son **SQL válido de Advantage Database Server**
y funcionaban con el ACE original; OpenADS las rechaza en el parser/executor.

Pregunta para FiveTech: ¿se mantendrá la compatibilidad con esta sintaxis de
ADS en OpenADS? Abajo, por cada caso, la consulta real, el error de OpenADS,
la causa raíz (archivo:línea) y el fix que aplicamos localmente (por si sirve
de base para incorporarlo upstream).

Las consultas salen de una misma pantalla (búsqueda de artículos):

```sql
SELECT {static} * FROM [articulo.dat] AS a
 WHERE 1 = 1
   AND UPPER(a.creffacart) <> 'N'
   AND UPPER(a.crefhabart) <> 'N'
   AND UPPER(a.cnombreart) LIKE 'A%'
 ORDER BY a.cnombreart
```

---

## 1. Hint de cursor `{static}`

- **ADS:** `SELECT {static} * FROM ...` — hint de tipo de cursor (cursor
  estático de solo lectura). ADS lo acepta y lo trata como directiva.
- **OpenADS:** `7200 expected '*' or column name in SELECT`
  (`src/sql/parser.cpp:765`). El parser ve `{` justo después de `SELECT`.
- **Causa:** el parser no contempla el prefijo `{ ... }` de hint de cursor.
- **Solución aplicada:** neutralizado en el ERP (es un no-op de optimización;
  estas consultas con `ORDER BY`/`JOIN` ya producen cursor estático). Lo ideal
  upstream sería que el parser **consuma e ignore** un bloque `{...}` opcional
  tras `SELECT`.

## 2. Tabla libre entre corchetes `[archivo.dat]`

- **ADS:** `FROM [articulo.dat]`, `FROM [mvtos\movim.dat]`,
  `FROM [planctas2026.dat]` — referencia canónica a *free table* por nombre
  de archivo/ruta entre corchetes.
- **OpenADS:** `7200 expected table name` (`src/sql/parser.cpp:1259`).
- **Causa:** el lector del FROM `read_identifier_or_filename()`
  (`parser.cpp:78`) no maneja `[...]`, aunque `read_identifier()`
  (`parser.cpp:94`) **sí** lo hace. Inconsistencia.
- **Fix aplicado** (espejo de `read_identifier`):

```cpp
std::string read_identifier_or_filename() {
    skip_ws();
    std::string out;
    if (pos_ < s_.size() && s_[pos_] == '[') {     // ADS free-table
        ++pos_;
        while (pos_ < s_.size() && s_[pos_] != ']')
            out.push_back(s_[pos_++]);
        if (pos_ < s_.size()) ++pos_;
        return out;
    }
    /* ... resto igual ... */
}
```

## 3. Alias de la tabla principal `FROM <tabla> AS a`

- **ADS:** `FROM [articulo.dat] AS a` (y luego `a.campo`). Válido.
- **OpenADS:** no consume el alias de la tabla principal: tras leer la tabla,
  el parser solo busca palabras `JOIN` (`parser.cpp:1190`). El ` AS a `
  sobrante hace que `WHERE`/`ORDER BY` no se reconozcan → filtro ignorado o
  error posterior.
- **Fix aplicado:** consumir un `AS <alias>` opcional tras la tabla principal
  (espejo del alias de derived-table):

```cpp
} else {
    stmt.table = c.read_identifier_or_filename();
    if (c.match_keyword("AS")) {
        c.read_identifier();   // alias parseado y descartado
    }
}
```

## 4. Función escalar `UPPER(col)` en el WHERE

- **ADS:** `WHERE UPPER(a.cnombreart) LIKE 'A%'`,
  `UPPER(a.creffacart) <> 'N'`. ADS evalúa la función por fila.
- **OpenADS:** el lado izquierdo de la comparación se lee como columna pelada
  (`parse_cmp`, `parser.cpp:239`); no soporta una llamada de función escalar.
- **Fix aplicado:** parsear `UPPER( <col> )` y marcar la comparación; en el
  executor se mapea a comparación **case-insensitive** (que es lo que
  `UPPER(col) op 'LITERAL_MAYUS'` significa). Se reusa el mecanismo existente
  `term.nocase` + `maybe_lower` del path plano (`ace_exports.cpp`):

```cpp
// parser: WhereCmp gana 'bool apply_upper'; parse_cmp detecta UPPER(...)
// executor (path plano single-table):
if ((conn_nocase || w.apply_upper) && !w.is_numeric) {
    term.nocase   = true;
    term.literal  = to_lower_ascii(term.literal);
    term.literal2 = to_lower_ascii(term.literal2);
}
```

> Nota: aplicado por ahora solo al path plano single-table (el de la pantalla).
> Los paths de JOIN/agregados tienen evaluadores separados y aún no honran
> `apply_upper`. Tampoco se hace upper de acentos CP850 (solo ASCII).

## 5. Predicado constante `WHERE 1 = 1`

- **ADS / SQL estándar:** `WHERE 1 = 1` (boilerplate de los generadores de SQL
  para concatenar `AND ...`).
- **OpenADS:** trata `1` como nombre de columna → `AE_COLUMN_NOT_FOUND`.
- **Fix aplicado:** en `parse_cmp`, si el LHS empieza con dígito (los campos
  xBase nunca empiezan con dígito) se evalúa el comparativo numérico en tiempo
  de parseo y se pliega a un nodo always-true (AND vacío) / always-false
  (OR vacío).

---

## 6. Join por coma multi-tabla (N≥3) con clave compuesta y wildcard calificado

Este es el caso **pendiente y de mayor peso**. El ERP arma un informe de
movimientos de inventario uniendo 5 tablas con la sintaxis SQL-89 de join por
coma (relaciones en el WHERE). ADS-SAP lo ejecuta sin problema (su AQE resuelve
el join multi-tabla); OpenADS no.

Origen en el ERP: `FUENTES\COMUNES\FUNCSSQL_MOD.PRG`, funciones
`AgrupaMoviHIstoricosSQL_ADS` (histórico, tablas `conseinv`/`moviminv`) y
`AgrupaMoviPorDiasSQL_ADS` (meses abiertos, tablas `trainv`/`movinv`). La
consulta real (histórico):

```sql
SELECT art.cclasi1art, art.cclasi2art, art.cclasi3art, art.cnombreart, art.lInvDisArt,
       art.cLinea1Art, ..., art.cTipIvaArt,
       cli.cnombrecli, cli.cdocnitcli, ..., num.nPlaDocTra, ..., num.cEstProTra,
       num.cHorTraTra, num.cMinTraTra, num.cSegTraTra, con.nFormatCon,
       mov.*
  FROM [concepto.dat] AS con, [conseinv.dat] AS num, [moviminv.dat] AS mov,
       [clientes.dat] AS cli, [articulo.dat] AS art
 WHERE ( art.ccodigoart = mov.ccodigoart )
   AND ( num.ccodigocli = cli.ccodigocli )
   AND ( num.ccodigocon = mov.ccodigocon AND num.cdocumetra = mov.cdocumetra )
   AND ( con.ccodigocon = num.ccodigocon )
   AND ( num.dfectratra >= {d '2026-01-01'} AND num.dfectratra <= {d '2026-01-31'} )
   AND ( con.cTipMovCon = '50' )
   AND ( mov.dfectratra >= {d '2026-01-01'} AND mov.dfectratra <= {d '2026-01-31'} )
```

Log del cliente (`adssqlerror.log`):

```
06/24/2026 09:43:56  7200  expected FROM   SELECT art.cclasi1art,...,con.nFormatCon, mov.*  FROM [concepto.dat] as con, [conseinv.dat] as num, ...
06/24/2026 10:41:09  7200  expected FROM   (idéntica)
```

El error visible ("expected FROM") es solo el **primero** de tres barreras
distintas. Para ejecutar esta consulta hay que resolver las tres:

### 6a. Wildcard calificado `<alias>.*` en la lista del SELECT

- **ADS:** `SELECT col1, col2, ..., mov.*` — proyecta columnas puntuales de
  varias tablas más **todas** las columnas de `mov`.
- **OpenADS:** `7200 expected FROM` (`src/sql/parser.cpp:1256-1257`). El parser
  de proyección lee items con `read_identifier()` (`parser.cpp:116`), que para
  `mov.*` devuelve `mov` y se detiene en `*` (el `*` no es alfanumérico). El
  bucle de proyección no encuentra `,`, corta, y el `match_keyword("FROM")`
  falla porque el cursor quedó sobre `*`.
- **Causa:** la lista de proyección no contempla la forma `<alias>.*`
  (sí soporta `SELECT *` pelado, `parser.cpp:782`, pero no el calificado).

### 6b. Join por coma de N≥3 tablas

- **ADS:** `FROM a, b, c, d, e` (5 tablas).
- **OpenADS:** soporta **exactamente 2** tablas por coma. Al ver la 3ª coma
  aborta con `comma-join supports exactly two tables; use INNER JOIN ... ON for
  more` (`src/sql/parser.cpp:1331-1335`). Aunque se reescribiera a
  `INNER JOIN ... ON`, **tampoco alcanzaría**: el AST guarda un único
  `std::optional<JoinClause> inner_join` (`src/sql/parser.h:239`) y el executor
  solo materializa un join de 2 tablas (hash-join a DBF temporal con prefijo
  `R_` en el lado derecho, `src/abi/ace_exports.cpp:13304`). El límite no es la
  sintaxis (coma vs `INNER JOIN`) sino el **motor**: no hay join de más de 2
  tablas.

### 6c. Clave de join compuesta

- **ADS:** `num.ccodigocon = mov.ccodigocon AND num.cdocumetra = mov.cdocumetra`
  (encabezado↔detalle por dos columnas).
- **OpenADS:** el lowering del comma-join junta las igualdades del WHERE y, si
  hay más de una, aborta con `comma-join supports a single equality join key;
  use INNER JOIN ... ON for composite keys` (`src/sql/parser.cpp:1442-1451`).
  `JoinClause` solo tiene `left_column`/`right_column` simples
  (`src/sql/parser.h:196-203`).

### Estado actual

El comma-join de **2 tablas con 1 igualdad** ya se agregó (parser
`parser.cpp:1307-1336` y lowering `1420-1463`; executor reusa el path de
`inner_join`). Lo que falta para el ERP real es: **N≥3 tablas + clave
compuesta + wildcard `<alias>.*`**, las tres juntas.

### Semántica ADS del resultado (importante para el diseño)

El cursor resultante expone las columnas con su **nombre sin calificar**
(`cclasi1art`, `nFormatCon`, y todas las de `moviminv` por `mov.*`); el ERP
luego hace `COPY TO <temp> VIA DBFCDX` y los informes leen esos campos por
nombre pelado. Es decir, el prefijo de alias (`art.`, `num.`) solo desambigua
en el SQL; **no** aparece en el campo de salida. El esquema `R_`-prefijado del
join de 2 tablas no modela esto y además no escala a N niveles (`R_R_...`).

### Diseño propuesto para upstream

1. **AST** (`parser.h`): reemplazar `table` único + `inner_join` único por una
   **lista de inputs** `[{name, alias}]` y una **lista de predicados de join**
   (cada uno una o más igualdades `alias_l.col_l = alias_r.col_r` → clave
   compuesta). Los items de proyección llevan `alias` opcional y un flag
   `wildcard` para `<alias>.*`.
2. **Parser**: (a) en proyección, aceptar `<alias>.*`; (b) en FROM, consumir la
   lista de tablas separadas por coma (cada una con `AS`/alias pelado);
   (c) extraer **todas** las igualdades `col = col` del WHERE (o del `ON`) como
   predicados de join multi-columna.
3. **Executor**: join **left-deep N-vías** en el orden del FROM — cada tabla
   nueva debe tener ≥1 igualdad que la conecte al rowset ya unido (hash sobre la
   concatenación de columnas de la clave cuando es compuesta). El WHERE residual
   se evalúa con resolución calificada por tabla. El esquema del cursor de salida
   = la proyección resuelta, con **nombres sin calificar** (semántica ADS) y
   `<alias>.*` expandido a los campos de esa tabla. Sustituye el merge
   `R_`-prefijado del path de 2 tablas, que no escala.

> Nota de paridad: el ERP usa `DISTINCT ON (...)` (PostgreSQL) en la rama
> equivalente `_XHB`; en la rama ADS lo resuelve después con `DedupTemMovADS`,
> así que **no** se pide `DISTINCT ON` a OpenADS.

### Implementación aplicada (2026-06-24)

Los tres gaps (6a/6b/6c) más el literal de fecha `{d '...'}` quedaron
implementados en el fuente de OpenADS. Resumen para upstream:

**`src/sql/parser.{h,cpp}`**
- `read_identifier()` gana out-params opcionales `out_alias` / `out_wildcard`
  (compatible hacia atrás: sin ellos, el comportamiento es idéntico). Captura
  el alias descartado y reconoce `<alias>.*`.
- `WhereCmp` gana `column_alias` y `outer_column_alias`; `parse_cmp` los
  rellena para LHS, UPPER()/LOWER() y la columna de join del RHS. El executor
  N-vías los necesita para distinguir columnas homónimas
  (`num.dfectratra` vs `mov.dfectratra`).
- `SelectStmt` gana `from_tables` (lista completa del FROM con alias),
  `select_items` (items de proyección: columna calificada o `<alias>.*`) y
  `projection_complex`. El FROM consume la coma-lista de N tablas; con 2 tablas
  se conserva el lowering al `inner_join` ya probado, con 3+ se delega al
  executor N-vías.
- Nuevo `read_odbc_temporal_literal()` para `{d 'YYYY-MM-DD'}` / `{ts ...}` /
  `{t ...}`, reducido a dígitos (`YYYYMMDD`) para comparar contra los bytes
  crudos del campo Date.

**`src/abi/ace_exports.cpp`** — en `AdsExecuteSQLDirect`, antes del path de
`inner_join`, un bloque que se activa con `from_tables.size() >= 3`:
- Abre las N tablas, mapea alias→índice.
- Extrae las igualdades `col = col` del WHERE como predicados de join
  (resueltos a (tabla, campo) en ambos lados).
- Arma un plan **left-deep** en orden del FROM: cada tabla nueva se conecta por
  ≥1 igualdad a una ya ligada (clave **compuesta** → hash sobre la concatenación
  de columnas).
- Resuelve el esquema de salida desde `select_items` con **nombres sin
  calificar** (semántica ADS); `<alias>.*` expande a los campos de esa tabla;
  de-dup first-wins.
- Nested-loop join con probes por hash; evalúa el WHERE residual calificado;
  materializa a DBF temporal (`_mjoin_<nanos>.dbf`) y lo reabre como cursor.

**Filter pushdown:** los predicados residuales del WHERE se clasifican por la
tabla más profunda que referencian y se evalúan en cuanto esa tabla se liga en
el walk, podando antes de descender a las dimensiones. Para el informe del ERP
esto significa que el rango de fecha (en `num`/`mov`) y `cTipMovCon` (en `con`)
recortan la explosión a sólo los documentos del mes/tipo pedido, en vez de
unir todo el histórico y filtrar al final.

Validado: 717/717 tests unitarios (incluye parse de la query real de 5 tablas
con clave compuesta + `alias.*` y el literal `{d}`). End-to-end en el ERP x64
(zeruswin sobre `openace64.dll`): el informe de movimientos de inventario corre
en **~3 s** (vs ~13 s sin pushdown y ~7 s con ADS-SAP comercial). DLL desplegada.

---

## Resumen

| # | Sintaxis ADS | Estado OpenADS | Fix |
|---|--------------|----------------|-----|
| 1 | `SELECT {static} *` | error 7200 | neutralizado en ERP (ideal: ignorar hint) |
| 2 | `FROM [tabla.dat]` | error 7200 | parser (brackets en read_identifier_or_filename) |
| 3 | `FROM t AS a` (tabla ppal) | alias no consumido | parser (consumir AS opcional) |
| 4 | `UPPER(col)` en WHERE | no soportado | parser + map a nocase (path plano) |
| 5 | `WHERE 1 = 1` | AE_COLUMN_NOT_FOUND | parser (plegar constante) |
| 6 | `FROM a,b` 2 tablas / 1 igualdad | soportado | parser + lowering al `inner_join` |
| 6a | `<alias>.*` en proyección | RESUELTO | parser (select_items + wildcard) |
| 6b | comma-join / `JOIN` de N≥3 tablas | RESUELTO | parser (from_tables) + executor N-vías |
| 6c | clave de join compuesta | RESUELTO | executor (hash compuesto, left-deep) |
| 7 | literal de fecha `{d 'YYYY-MM-DD'}` | RESUELTO | parser (read_odbc_temporal_literal) |

Fixes 2-7 implementados en `src/sql/parser.{h,cpp}` y `src/abi/ace_exports.cpp`
(ver "Implementación aplicada (2026-06-24)" arriba). 717/717 tests unitarios OK.
