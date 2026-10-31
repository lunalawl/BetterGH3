@echo off
cd /d %~dp0
title Allow through Firewall

net session >nul 2>&1
if %errorlevel% neq 0 (
    echo Requesting administrator privileges...
    powershell -Command "Start-Process '%~f0' -Verb RunAs"
    exit /b
)

set "TARGET_REL=..\GH3.exe"
for %%I in ("%TARGET_REL%") do set "TARGET_PATH=%%~fI"

if not exist "%TARGET_PATH%" (
    echo ERROR: Could not find Guitar Hero III
    pause
    popd
    exit /b 1
)

set "RULE_PROG_IN=Guitar Hero III (program, inbound)"
set "RULE_PROG_OUT=Guitar Hero III (program, outbound)"
set "RULE_PORT_IN=Demonware (port, inbound)"
set "RULE_PORT_OUT=Demonware (port, outbound)"

netsh advfirewall firewall delete rule name="%RULE_PROG_IN%" >nul 2>&1
netsh advfirewall firewall delete rule name="%RULE_PROG_OUT%" >nul 2>&1
netsh advfirewall firewall delete rule name="%RULE_PORT_IN%" >nul 2>&1
netsh advfirewall firewall delete rule name="%RULE_PORT_OUT%" >nul 2>&1

echo Adding firewall rules for GH3.exe ...
netsh advfirewall firewall add rule name="%RULE_PROG_IN%" ^
    dir=in action=allow program="%TARGET_PATH%" enable=yes profile=any ^
    description="Allow inbound traffic for Guitar Hero III." >nul 2>&1
netsh advfirewall firewall add rule name="%RULE_PROG_OUT%" ^
    dir=out action=allow program="%TARGET_PATH%" enable=yes profile=any ^
    description="Allow outbound traffic for Guitar Hero III." >nul 2>&1

echo Adding firewall rules for UDP port 3074 (Demonware) ...
netsh advfirewall firewall add rule name="%RULE_PORT_IN%" ^
    dir=in action=allow protocol=UDP localport=3074 enable=yes profile=any ^
    description="Allow inbound UDP port 3074 for Demonware services." >nul 2>&1
netsh advfirewall firewall add rule name="%RULE_PORT_OUT%" ^
    dir=out action=allow protocol=UDP localport=3074 enable=yes profile=any ^
    description="Allow outbound UDP port 3074 for Demonware services." >nul 2>&1

echo.
echo Firewall rules added successfully:
echo   - %RULE_PROG_IN%
echo   - %RULE_PROG_OUT%
echo   - %RULE_PORT_IN%
echo   - %RULE_PORT_OUT%
echo.
pause

popd
exit /b 0
