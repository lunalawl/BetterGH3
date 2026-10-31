@echo off
setlocal enabledelayedexpansion
cd /d "%~dp0"

call :CompilePak patch
call :CompilePak patch_text
call :CompilePak patch_text_f
call :CompilePak patch_text_g
call :CompilePak patch_text_i
call :CompilePak patch_text_s

goto :eof

:CompilePak
if exist "%~dp0..\game\DATA\%~1.pak.xen" del "%~dp0..\game\DATA\%~1.pak.xen"
honeycomb pak compile "%~dp0%~1" -g gh3 -c pc -o "%~dp0..\game\DATA"
exit /b
