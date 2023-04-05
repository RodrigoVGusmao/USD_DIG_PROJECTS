setlocal EnableDelayedExpansion

@echo off

set "file=Copyright"
set /a "segment=0"

for /f "tokens=*" %%a in (FILEFMTS.LST) do (
	set "line=%%~a"
	set "text=%%~a"
	set "startChar=!line:~0,1!"
	set "endChar=!line:~-1,1!"
	if !startChar!==- (
		if !endChar!==- (
			set "line=!line:~9!"
			for /f "delims=-" %%i in ("!line!") do (
				set "file=%%~i"
				set "file=!file:/=-!"
				set /a "segment+=1"
			)
		)
	)
	echo:!text!>>chapters\seg_!segment!_!file!.txt
)