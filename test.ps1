$Program = ".\main.exe"

Write-Host "Running $Program"
$output = & $Program 2>&1
$actualExitCode = $LASTEXITCODE

echo "output: [$output]"
echo "actualExitCode: [$actualExitCode]"
