@echo off
setlocal enabledelayedexpansion
cd /d "%~dp0"

call :CompilePak customs

goto :eof

:CompilePak
if exist "%~dp0..\game\DATA\%~1.pak.xen" del "%~dp0..\game\DATA\%~1.pak.xen"
honeycomb pak compile "%~dp0%~1" -g gh3 -c pc -o "%~dp0..\game\DATA"
exit /b
