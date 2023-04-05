setlocal EnableDelayedExpansion

certutil -encodehex binDec.txt binOut.txt
certutil -decodehex hexDec.txt hexOut.txt

for /f "tokens=2-17" %%a in (binOut.txt) do (
set "text=%%a %%b %%c %%d %%e %%f %%g %%h %%i %%j %%k %%l %%m %%n %%o %%p"
echo.!text!>>test.txt
)

pause