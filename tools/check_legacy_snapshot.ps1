[CmdletBinding()]
param()

$ErrorActionPreference = 'Stop'
$repositoryRoot = Split-Path -Parent $PSScriptRoot
$snapshot = Join-Path $repositoryRoot 'cpp.json'
$expected = 'A7E51CC022C93270BD0574BBD025498A6E004EC0891064B24809329305DD76A0'
$actual = (Get-FileHash -LiteralPath $snapshot -Algorithm SHA256).Hash

if ($actual -ne $expected) {
  throw "cpp.json changed: expected $expected, got $actual"
}

Write-Host "cpp.json unchanged: $actual"
