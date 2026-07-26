# CONTINUAR — Aportes Russoft a OpenADS (handoff, última act. 2026-06-24)

Doc de continuación. Contexto completo en la memoria `project_openads_ace_replacement.md`
y en `RUSSOFT_ADS_SYNTAX_GAPS.md`. La DLL desplegada (`F:\zerus64\openace64.dll` y
`C:\OpenADS\openace64.dll`) ya tiene TODO lo de abajo y está LIMPIA (sin instrumentación).

---

## Estado: COMPLETADO y desplegado (717/717 tests)

Corriendo el ERP Harbour/FiveWin (Russoft/Zerus) sobre OpenADS (reemplazo de ace64):

1. **Join N-vías** (`parser.{h,cpp}`, `ace_exports.cpp`): comma-join 3+ tablas,
   clave compuesta, `<alias>.*`, columnas calificadas, literal `{d}`. Filter
   pushdown + hash-build filter. Informe de inventario: **13s → 3s**.
2. **INDEXAR** (`cdx_index.{h,cpp}`, `ace_exports.cpp`): `build_bulk` (sort-then-build),
   sibling re-park, memoize tokenize. conseinv 10 tags: **15min → 47s (~19x)**.
3. **Tabla vacía al filtrar** (`index_expr.cpp`): `apply_scalar_fn` compartida
   (key + FOR evaluator) con SUBS/AT/ATNUM/VAL/TRIM. RESUELTO.
4. **Navegación browse** (`ace_exports.cpp`, `cdx_index.{h,cpp}`): cache O(1) de
   posición (scrollbar ya no camina el índice por pintada) + EOF→fondo. RESUELTO
   (navegación normal y PageDown sostenido OK).

Build: `"C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build C:\OpenADS\OpenADS-main\build\default --config Release --parallel`
Tests: `build\default\tests\Release\openads_unit_tests.exe`. Deploy: copiar `openace64.dll` a F:\zerus64 + C:\OpenADS.

---

## PENDIENTE 1 — Subir el PR a FiveTech (ya casi listo)

FiveTech pidió el PR. Upstream: `github.com/FiveTechSoft/OpenADS` (Apache-2.0).

**Ya hecho:** commit `97cab50` en la rama **`russoft-erp-compat`** del repo git
`C:\OpenADS\upstream_clone` (clon real del upstream), sobre el baseline
`0a20c23` (= el estado del zip, ANTES del feat NTX-numeric `5f247c5`). El diff
del commit son SOLO nuestros 8 archivos, limpio.

**Falta (necesita la cuenta GitHub del usuario):**
1. Usuario **forkea** `FiveTechSoft/OpenADS`.
2. `cd C:\OpenADS\upstream_clone && git remote add fork git@github.com:<USUARIO>/OpenADS.git && git push fork russoft-erp-compat`
3. Abrir PR contra `FiveTechSoft/OpenADS:main`. El diff del PR es limpio (nuestros
   cambios vs el merge-base = baseline). **PERO** al mergear va a haber CONFLICTOS
   con el feat NTX-numeric reciente (commits `5f247c5`/`523c77d`) en
   `ace_exports.cpp` (dentro de `AdsCreateIndex61`, donde el NTX agrega
   `ntx_numeric_key`/`ntx_num_width` y nosotros agregamos el cableado de
   `build_bulk`) y `index_expr.cpp`. Resolver: combinar AMBOS (el NTX por encoding
   de clave NTX + nuestro `build_bulk` para CDX coexisten en el build loop).
   Alternativa: `git rebase origin/main` la rama y resolver los conflictos antes
   del push. (`gh` NO está instalado; usar git + la cuenta del usuario.)

PR description: usar `RUSSOFT_ADS_SYNTAX_GAPS.md` + el mensaje del commit `97cab50`.

---

## PENDIENTE 2 — ART/CLI/PLA en BuscaRegistro (browse de temporal SQL)

`BuscaRegistro` (FW_FUNCSST3.PRG:1541), para alias ART/CLI/PLA, hace:
`ADSRunSQL("ART", "Select * From [articulo.dat] as a WHERE 1=1 AND UPPER(...) LIKE 'X%' order by a.cnombreart")`
→ `INDEX ON cNombreArt` → `DBSETORDER(1)` → browse. (tipo ADS_CDX; flag
`EXTENSION_ADS_ADI`=.F. en ACTUALIZ.PRG:2298). Síntoma: navega por recno
(desordenado), sin filtro.

**Diagnóstico (log abi, confirmado):** el cursor "ART" tiene **6 índices**
(`ordered_count=6`) y navega con **`order=0`** → es `articulo.dat` FUENTE con sus
6 tags de producción, NO una temporal limpia. El `DBSETORDER(1)` agarra un tag
que no corresponde. El bench REPRODUCE la query EXACTA (brackets+alias+
calificadas+UPPER+ORDER BY) y **materializa limpio** (500 filtrados, INDEX ON +
nav OK) → **NO es bug del motor en aislamiento**; es específico de articulo.dat
con CDX de producción (6 tags) + rddads.

**Fix recomendado (lado ERP, simple y probado):** en el branch ART/CLI/PLA, tras
`ADSRunSQL`, `COPY TO temp VIA "DBFCDX"` → abrir la temporal limpia → `INDEX ON`
+ browse sobre ESA (patrón de `AgrupaMoviHIstoricosSQL_ADS`). Da un cursor
self-contained sin los 6 tags → navega ordenado.

**Alternativa (lado OpenADS, más profundo, no reproducible en bench aún):** forzar
que el executor MATERIALICE el SELECT de una tabla (con WHERE/ORDER BY) en una
temporal limpia en vez de devolver la fuente con sus índices. Requiere instrumentar
el executor en la corrida real para confirmar por qué devuelve la fuente.

Para reproducir/diagnosticar: el bench `tests/bench/dbf_cdx_bench.cpp` tuvo una
sección de repro (revertida); re-agregar `AdsExecuteSQLDirect`+`AdsCreateIndex`
sobre el cursor si hace falta.

---

## Prompt para retomar (pegar en sesión nueva)

> Retomamos los aportes Russoft a OpenADS (memoria `project_openads_ace_replacement`
> y `C:\OpenADS\OpenADS-main\CONTINUAR_RUSSOFT.md`). Todo el trabajo OpenADS (join
> N-vías, INDEXAR 19x, tabla-vacía FOR, navegación browse O(1)+EOF) está hecho,
> 717/717 tests, desplegado y commiteado en la rama `russoft-erp-compat` de
> `C:\OpenADS\upstream_clone` (commit 97cab50, sobre baseline 0a20c23). Faltan dos
> cosas: (1) el PR a `github.com/FiveTechSoft/OpenADS` — ya forkeé/voy a forkear;
> ayudame con el push a mi fork y a resolver el conflicto con el feat NTX-numeric
> reciente en ace_exports.cpp/index_expr.cpp. (2) El browse ART/CLI/PLA de
> BuscaRegistro (SQL→temporal→browse): aplicar el fix lado-ERP (COPY TO temporal
> limpia tras ADSRunSQL, patrón AgrupaMoviHIstoricosSQL_ADS) en FW_FUNCSST3.PRG.
