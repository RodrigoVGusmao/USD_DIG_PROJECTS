@echo off
dir /ad /on /b
echo:
echo:filename:
set /p asmProject=""
IF NOT EXIST %asmProject% (
	mkdir %asmProject%
	type nul>%asmProject%\%asmProject%.asm
)

set asmFile=%asmProject%.asm
set objFile=%asmProject%.obj
set exeFile=%asmProject%.exe
set "windows=kernel32.dll user32.dll gdi32.dll"
set "crt=kernel32.dll msvcrt.dll"

set path=^
E:\Users\digao\AppData\Local\bin\NASM;^
E:\Users\digao\AppData\Local\bin\GoLink;^
E:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.32.31326\bin\Hostx64\x64;^
E:\Program Files\CodeBlocks\MinGW\bin;^
%~dp0zBatchResources;%path%

cd %asmProject%
start "nasm / Go Link" cmd.exe /k "@echo off && echo:syntax="nasm[Win64 ^| Win32 ^| Bin]" && echo:syntax="link[Console ^| Dll ^| Default] ^<entry^> ^<resources^>" && echo:syntax="listFullSyntax" && echo: && echo:(*) - optional && echo:[*] - program information && echo:frequent resources: kernel32.dll, gdi32.dll, user32.dll, msvcrt.dll && echo:"
start "execute" cmd.exe /k "@echo off && echo:"syntax:run" && echo: && echo:variable names:asmProject, asmFile, objFile, exeFile"

moveWindow 953 512 117 30 notepad %~dp0\%asmProject%\%asmFile%