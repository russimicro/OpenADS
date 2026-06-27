# OpenADS — Plan de ejecución (2026-06-27)

Hoja de ruta acordada tras la revisión del proyecto. Los ítems marcados con `[x]`
están hechos en esta sesión o antes; el resto sigue el orden de prioridad.

> Backlog técnico detallado: `TODO.md` en la raíz del repo.

---

## Fase 0 — Endurecer lo desplegado (hecho v1.5.1)

- [x] Path jail en `Connect` remoto (`resolve_under_root`)
- [x] LockMgr: unlock anidado coherente con refcount
- [x] ABI remoto: `AdsGetMemoDataType`, `AdsSetStringW`, `AdsSetJulian`, `AdsSetFieldRaw`
- [x] TLS: verificación por defecto; `OPENADS_TLS_INSECURE=1` para dev
- [x] Límite `kMaxWireFields` en cliente wire
- [x] `creds_mu_` thread-safe en servidor
- [x] Actualizar `docs/known-issues.md` y sección VFP en `TODO.md`

---

## Fase 1 — Confianza en CI (casi listo)

### 1.1 Harbour smoke en CI Windows `[x]` local / `[ ]` CI

- [x] `tools/scripts/run_harbour_smoke.ps1` — script portable (fix PS5.1: sin em-dash)
- [x] `tools/scripts/bootstrap_harbour_ci.ps1` — instala Harbour en caché CI
- [x] `tests/smoke/harbour/smoke.hbp` — proyecto hbmk2
- [x] Job `harbour-smoke` en `.github/workflows/ci.yml`
- [x] Verificado local: `EXIT=0` con `C:\harbour` + `build\default`
- [ ] Calentar caché Harbour en primer run de CI (puede tardar ~45-90 min una vez)
- [ ] Opcional: self-hosted runner con `C:\harbour` preinstalado

**Criterio de éxito:** `smoke.exe` sale con código 0 en `windows-2022` tras cada push a `main`.

### 1.2 Remote `AdsSetRelation` `[x]` core / `[ ]` extras

- [x] Estado de relaciones por `ADSHANDLE` (local + `tcp://`)
- [x] `active_index_id` en `RemoteTable` para seek de hijo
- [x] `apply_relations_for_handle()` tras nav remota (Skip / Goto*)
- [x] Test unitario loopback `tcp://127.0.0.1` (4/4 tests, 127 aserciones)
- [x] Fix firma `AdsOpenTable` en test wire (9 args, no 5)
- [ ] `AdsSetScopedRelation` remoto con scopes (mismo motor; seek scoped ya en código)
- [ ] Test Harbour con `SET RELATION` sobre servidor remoto
- [ ] Caso local parent → remote child (hoy cubierto remote→remote)

**Criterio de éxito:** tests `abi_set_relation_test` verdes incl. caso wire — **cumplido local**.

---

## Fase 2 — Cerrar gaps remotos (siguiente mes)

| # | Tarea | Prioridad | Esfuerzo |
|---|-------|-----------|----------|
| 2.1 | `AdsAggregate` / `AdsFetchWhere` en ABI local (opcodes wire ya existen) | Alta | 2–3 d |
| 2.2 | `AdsSetRecord` remoto | Alta | 2 d |
| 2.3 | `AdsCustomizeAOF` remoto | Media | 1–2 d |
| 2.4 | Fixtures ADT/ADI en repo + tests CI sin skip | Alta | 1 d |
| 2.5 | VFP header `0x32` (autoinc + NULL juntos) + test | Media | 2 d |

---

## Fase 3 — Decisión Data Dictionary (estratégico)

**Decisión recomendada:** DD propio OpenADS (objetos + JSON en `.am`) como formato oficial;
import SAP una sola vez vía `tools/import_dd`; no perseguir round-trip binario SAP.

| # | Tarea |
|---|-------|
| 3.1 | Documento de diseño DD v2 (objetos, permisos bitmask, caché en servidor) |
| 3.2 | Migrar `import_dd` al formato v2 |
| 3.3 | Suite de tests DD v2 (usuarios, grupos, RI, triggers, permisos efectivos) |
| 3.4 | DaWeb: leer solo DD v2; dejar de depender de blobs SAP |

**No hacer ahora:** decodificar blobs de permisos SAP sin instancia ADS 11.x viva.

---

## Fase 4 — Compatibilidad SAP (solo si hay caso de uso)

| # | Tarea | Bloqueador |
|---|-------|------------|
| 4.1 | Matriz `rddtst` vs ADS 11.x/12.x | Instancia SAP + wire SAP |
| 4.2 | `SapWireTransport` (`ads://` / `sap://`) | Clean-room wire spec |
| 4.3 | `docs/ace-coverage.md` con divergencias medidas | 4.1 |

---

## Fase 5 — Producto y docs

| # | Tarea |
|---|-------|
| 5.1 | Release v1.5.1 con changelog (seguridad + CI smoke + SetRelation remoto) |
| 5.2 | DDL `ALTER`/`DROP` — enganchar hooks backend (ya parseado en v1.5.0) |
| 5.3 | DaWeb: RI dropdown, grupos de usuario, grupos `DB:Public`… |
| 5.4 | Harbour smoke Linux en CI (después de Windows estable) |
| 5.5 | README: `openace64.dll` rename, `bindings/php` vs `php_ext` |

---

## Orden de trabajo inmediato (esta sesión)

1. ~~Plan completo en `docs/plan-ejecucion.md`~~
2. ~~Harbour smoke CI (Windows) — scripts + job CI; smoke local OK~~
3. ~~Remote `AdsSetRelation` — implementado + tests wire OK~~
4. ~~Build + tests SetRelation verdes~~ (`openads_unit_tests --test-case="*SetRelation*"`)
5. Push a `main` y confirmar job `harbour-smoke` en GitHub Actions
6. Commit / tag sugerido: `v1.5.1` o rc

---

## Referencias

- Plan original de revisión: conversación 2026-06-27
- `TODO.md` — backlog técnico detallado (DD, VFP, SQL…)
- `roadmap.txt` — hitos históricos SAP vs OpenADS
- `docs/known-issues.md` — estado v1.5.0+