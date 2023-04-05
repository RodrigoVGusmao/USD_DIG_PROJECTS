SETLOCAL EnableDelayedExpansion

for /f "tokens=1-7" %%a in (names.txt) do (
	set "name=%%~a"
	echo:!name!
)>>newNames.txt