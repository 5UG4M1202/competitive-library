[CmdletBinding()]
param(
  [string]$Compiler = ''
)

$ErrorActionPreference = 'Stop'
$repositoryRoot = Split-Path -Parent $PSScriptRoot
$tests = @(Get-ChildItem -LiteralPath (Join-Path $repositoryRoot 'verify\local') -Filter '*.test.cpp')
if ($tests.Count -eq 0) {
  throw 'No local C++ tests found.'
}

if ([string]::IsNullOrWhiteSpace($Compiler)) {
  foreach ($candidate in @('g++', 'clang++', 'cl')) {
    if (Get-Command $candidate -ErrorAction SilentlyContinue) {
      $Compiler = $candidate
      break
    }
  }
}
if ([string]::IsNullOrWhiteSpace($Compiler)) {
  throw 'No C++ compiler found. Run from a Visual Studio Developer PowerShell or install g++/clang++ explicitly.'
}

$tempRoot = [IO.Path]::GetTempPath()
$outputDirectory = Join-Path $tempRoot ("competitive-library-tests-" + [guid]::NewGuid().ToString('N'))
New-Item -ItemType Directory -Path $outputDirectory | Out-Null

try {
  foreach ($test in $tests) {
    $executable = Join-Path $outputDirectory ($test.BaseName + '.exe')
    $objectFile = Join-Path $outputDirectory ($test.BaseName + '.obj')
    if ((Split-Path -Leaf $Compiler) -in @('cl', 'cl.exe')) {
      & $Compiler /nologo /std:c++20 /EHsc /W4 /permissive- $test.FullName "/Fo:$objectFile" "/Fe:$executable"
    } else {
      & $Compiler -std=c++20 -O2 -Wall -Wextra -Wshadow -Wconversion -pedantic $test.FullName -o $executable
    }
    if ($LASTEXITCODE -ne 0) { throw "Compilation failed: $($test.Name)" }
    & $executable
    if ($LASTEXITCODE -ne 0) { throw "Test failed: $($test.Name)" }
    Write-Host "PASS $($test.Name)"
  }
} finally {
  $resolved = [IO.Path]::GetFullPath($outputDirectory)
  if ($resolved.StartsWith([IO.Path]::GetFullPath($tempRoot), [StringComparison]::OrdinalIgnoreCase) -and
      (Test-Path -LiteralPath $resolved)) {
    Remove-Item -LiteralPath $resolved -Recurse -Force
  }
}
