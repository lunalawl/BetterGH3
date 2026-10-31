@echo off
setlocal enabledelayedexpansion
cd /d "%~dp0"

call :CompileBasePak qb
call :CompileBasePak qb_f
call :CompileBasePak qb_g
call :CompileBasePak qb_i
call :CompileBasePak qb_k

goto :eof

:CompileBasePak
if exist "%~dp0..\game\DATA\PAK\%~1.pak.xen" del "%~dp0..\game\DATA\PAK\%~1.pak.xen"
honeycomb pak compile "%~dp0\base_updated_scripts\%~1" -g gh3 -c pc -o "%~dp0..\game\DATA\PAK"
exit /b
