SETLOCAL EnableDelayedExpansion

for /f "tokens=1,2" %%a in (kit.txt) do (
	set "command=%%~a"
	set "kit=%%~b"
	if !command!==Template echo:!kit!
)>>newKit.txt