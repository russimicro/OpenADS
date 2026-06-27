---
title: Known issues
layout: default
nav_order: 9
---

# Known issues — current

Status as of **v1.5.0** (2026-06-27).

## Open

### SAP ACE wire protocol

OpenADS speaks its own documented wire protocol on `tcp://` / `tls://`.
It is **not** byte-compatible with the proprietary SAP Advantage 11.x/12.x
TCP protocol. Talking to a legacy ADS server as a drop-in client requires
a future `SapWireTransport` layer (`ads://` / `sap://` URIs).

### SAP-imported Data Dictionary permissions

For `.add` files created by SAP Data Architect, per-table group
permission levels are encoded in encrypted 8-byte blobs that OpenADS
cannot decode yet. Imported DDs may show full DML for every group
where SAP shows read-only access. Use `pmsys_imported.add` (via
`tools/import_dd`) or grant permissions from OpenADS-native tooling.

### TLS certificate verification

`tls://` verifies peer certificates by default. Self-signed or
private-CA endpoints require either a CA bundle (future
`AdsSetTlsCa` entry point) or the dev-only environment variable
`OPENADS_TLS_INSECURE=1`.

### Server-side TLS termination

Client-side TLS (`tls://` in `ace64.dll`) is implemented via mbedtls.
`openads_serverd` does not terminate TLS natively — front it with
nginx, Caddy, or stunnel. See [TLS deployment](en/tls-deployment/).

### Studio LocalServer auth

LocalServer mode (Studio embedded in `ace64.dll` / `ace32.dll`) has no
HTTP Basic auth. The default bind is `127.0.0.1`; if you set
`OPENADS_STUDIO_HOST=0.0.0.0`, put the console behind a reverse proxy
that handles authentication. Remote Server mode (`openads_serverd`)
supports `--http-user user:password`.

### Remote ABI gaps

Some `Ads*` entry points still return `AE_FUNCTION_NOT_AVAILABLE` on
`tcp://` remote tables while the local path works:

- `AdsSetRelation` / `AdsSetScopedRelation`
- `AdsSetRecord`
- `AdsCustomizeAOF`
- `AdsGetRecordCRC`
- `AdsAggregate` / `AdsFetchWhere` (local in-process; wire opcodes exist)

### VFP combined header (0x32)

Autoinc, V/Q types, and NULL-bitmap work separately. Tables that combine
autoinc **and** nullable columns under the VFP `0x32` header signature
may not parse correctly yet.

### DDL execution

`ALTER TABLE`, `DROP TABLE`, and `DROP INDEX` are parsed (v1.5.0) but
backend execution hooks are not wired yet.

## Closed recently

- **Path traversal on remote Connect** — fixed v1.5.1: client paths are
  canonicalized and jailed under `openads_serverd --data`.
- **LockMgr nested unlock** — fixed v1.5.1: OS byte locks stay held until
  the final nested `unlock_*`.
- **Remote memo/Unicode/date/raw field writes** — fixed v1.5.1:
  `AdsGetMemoDataType`, `AdsSetStringW`, `AdsSetJulian`, `AdsSetFieldRaw`
  route through `tcp://`.
- **Wire-protocol forward-only prefetch** — re-enabled v1.0.3 (PR #47);
  sequential prefetch on `Skip(+N)` with cursor resync.
- **AdsEval*Expr / AdsStmt*** — implemented; no longer stubs.
- **X# AXDBFCDX RDD** — local + remote smoke passes (rc19).

See `CHANGELOG.md` for the full per-release breakdown.