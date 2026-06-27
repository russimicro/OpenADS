# Build and run tests/smoke/harbour/smoke.prg against OpenADS ace64.
#
# Environment:
#   OPENADS_BUILD  - CMake build dir (default: build/msvc-x64)
#   HARBOUR_ROOT   - Harbour install root (default: C:\harbour or harbour-ci)
#   OPENADS_SKIP_HARBOUR_SMOKE=1 - exit 0 without running (local opt-out)

param(
    [string]$OpenAdsBuild = $(if ($env:OPENADS_BUILD) { $env:OPENADS_BUILD }
                              else { Join-Path $PSScriptRoot "..\..\build\msvc-x64" }),
    [string]$HarbourRoot  = $env:HARBOUR_ROOT
)

$ErrorActionPreference = "Stop"
$repo = (Resolve-Path (Join-Path $PSScriptRoot "..\..")).Path
$smokeDir = Join-Path $repo "tests\smoke\harbour"

if ($env:OPENADS_SKIP_HARBOUR_SMOKE -eq "1") {
    Write-Host "[harbour-smoke] Skipped (OPENADS_SKIP_HARBOUR_SMOKE=1)"
    exit 0
}

if (-not $HarbourRoot) {
    foreach ($c in @("C:\harbour", "$env:RUNNER_TEMP\harbour-ci")) {
        if (Test-Path (Join-Path $c "bin\win\msvc64\hbmk2.exe")) {
            $HarbourRoot = $c
            break
        }
    }
}
if (-not $HarbourRoot -or -not (Test-Path (Join-Path $HarbourRoot "bin\win\msvc64\hbmk2.exe"))) {
    Write-Error "[harbour-smoke] Harbour not found. Set HARBOUR_ROOT or run bootstrap_harbour_ci.ps1"
}

$aceLib = Join-Path $OpenAdsBuild "src\Release"
$makeCdx = Join-Path $OpenAdsBuild "tests\Release\make_cdx.exe"
if (-not (Test-Path $makeCdx)) {
    Write-Error "[harbour-smoke] make_cdx.exe missing - build OpenADS first: $makeCdx"
}

$env:HB_INSTALL = $HarbourRoot
$env:OPENADS_LIB = $aceLib
$hbBin = Join-Path $HarbourRoot 'bin\win\msvc64'
$env:PATH = "$hbBin;$aceLib;$env:PATH"

Push-Location $smokeDir
try {
    Write-Host "[harbour-smoke] Generating data.dbf ..."
    powershell -ExecutionPolicy Bypass -File (Join-Path $smokeDir "make_data.ps1")

    Write-Host "[harbour-smoke] Generating data.cdx ..."
    & $makeCdx data.cdx
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

    Write-Host "[harbour-smoke] hbmk2 build ..."
    hbmk2 -comp=msvc64 smoke.hbp
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

    Write-Host "[harbour-smoke] Running smoke.exe ..."
    & .\smoke.exe
    $code = $LASTEXITCODE
    Write-Host "[harbour-smoke] EXIT=$code"
    exit $code
}
finally {
    Pop-Location
}