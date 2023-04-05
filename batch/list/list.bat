@echo off
set /a "identifier=0"
:start
set /p "product=insira um produto:"
if /i %product%==sair goto eof
if /i %product%==exit goto eof

set /a "value=%RANDOM%%%16"
set "price=%value%"
set /a "value=%RANDOM%%%100"
set "price=%price%.%value%"

set /a "quantity=%RANDOM%%%1024"

echo:%identifier%	%product%	%price%	%quantity%>>"%~dp0produtos.txt"
set /a "identifier+=1"

goto start