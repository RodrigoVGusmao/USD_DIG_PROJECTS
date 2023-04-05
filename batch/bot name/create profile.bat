@echo off
SETLOCAL EnableDelayedExpansion

FOR /F "" %%I IN ('FIND /C /V "" ^<newNames.txt') DO SET /A nameLines=%%I
FOR /F "" %%I IN ('FIND /C /V "" ^<newKit.txt') DO SET /A kitLines=%%I

set "skill=Elite"

:reloop
set /p "input=How many characters of difficulty %skill%? "
set /a "num=input"
if %num% lss 1 goto jumpoff

set /a "iterator=0"

:loop
SET /A nameSkip=%RANDOM%%%nameLines

<newNames.txt (
	FOR /L %%I IN (1,1,%nameSkip%) DO (
		SET /P nameLine=
	)
	SET nameLine=
	SET /P nameLine=
)

SET /A kitSkip=%RANDOM%%%kitLines

<newKit.txt (
	FOR /L %%I IN (1,1,%kitSkip%) DO (
		SET /P kitLine=
	)
	SET kitLine=
	SET /P kitLine=
)

choice /m "use "%kitLine%" kit for "%nameLine%"? y/n:"
if %ERRORLEVEL%==2 goto loop

set /a "iterator+=1"

(
	echo:%skill%+%kitLine% %nameLine%
	set /A var=%RANDOM%%%4+1
	echo:	Skin = !var!
	set /A var2=%RANDOM%%%30+80
	echo:	VoicePitch = !var2!
	echo:End
	echo:
)>>profileData.txt

if %iterator% neq %num% goto loop

:jumpoff
if %skill%==Elite set "skill=Expert" & echo://---------------------------------------->>profileData.txt & goto reloop
if %skill%==Expert set "skill=VeryHard" & echo://---------------------------------------->>profileData.txt & goto reloop
if %skill%==VeryHard set "skill=Hard" & echo://---------------------------------------->>profileData.txt & goto reloop
if %skill%==Hard set "skill=Tough" & echo://---------------------------------------->>profileData.txt & goto reloop
if %skill%==Tough set "skill=Normal" & echo://---------------------------------------->>profileData.txt & goto reloop
if %skill%==Normal set "skill=Fair" & echo://---------------------------------------->>profileData.txt & goto reloop
if %skill%==Fair set "skill=Easy" & echo://---------------------------------------->>profileData.txt & goto reloop