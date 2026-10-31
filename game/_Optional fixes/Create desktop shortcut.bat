@echo off
setlocal
cd /d "%~dp0"
title Create desktop shortcut

net session >nul 2>&1
if %errorlevel% neq 0 (
    echo Requesting administrator privileges...
    powershell -Command "Start-Process '%~f0' -Verb RunAs"
    exit /b
)

set "TARGET=%~dp0..\GH3.exe"
set "WORKDIR=%~dp0.."
set "SHORTCUT_NAME=Guitar Hero III"

powershell -NoProfile -ExecutionPolicy Bypass -Command ^
  "$ws = New-Object -ComObject WScript.Shell; " ^
  "$desktop = [Environment]::GetFolderPath('CommonDesktopDirectory'); " ^
  "$startMenu = Join-Path $env:ProgramData 'Microsoft\Windows\Start Menu\Programs'; " ^
  "$target = '%TARGET%'; " ^
  "$workDir = '%WORKDIR%'; " ^
  "$icon = '%TARGET%,0'; " ^
  "$links = @((Join-Path $desktop '%SHORTCUT_NAME%.lnk'), (Join-Path $startMenu '%SHORTCUT_NAME%.lnk')); " ^
  "foreach ($l in $links) { $s = $ws.CreateShortcut($l); $s.TargetPath = $target; $s.WorkingDirectory = $workDir; $s.IconLocation = $icon; $s.Save() }"

echo Desktop and Start Menu shortcuts created successfully!
echo.

pause
