setlocal EnableDelayedExpansion

@echo off

CD.>outputTable.txt
CD.>outputProcessedTable.txt
CD.>outputProcessedTableInstruction.txt

SET /A "prevNum=0"
SET /A "MultiOpcodeIndex=-1"

FOR /F "tokens=* delims=" %%A IN (table.txt) DO (
	SET string=%%A

	rem type C arguments = 2, 4 or 8 bytes
	SET string=!string:imm16/32/64=C!

	rem type F arguments = 2 or 4 bytes
	SET string=!string:imm16/32=F!
	SET string=!string:rel16/32=F!

	rem type B arguments = 1 byte
	SET string=!string:imm8=B!
	SET string=!string:rel8=B!

	rem type D arguments = 2 byte
	SET string=!string:imm16=D!

	rem type A arguments = 1 to 6 byte
	SET string=!string:r/m8=A!
	SET string=!string:r8=A!
	SET string=!string:r/m16/32/64=A!
	SET string=!string:r16/32/64=A!
	SET string=!string:r/m16/32=A!
	SET string=!string:r/m64/16=A!
	SET string=!string:r32/64=A!
	SET string=!string:r/m32=A!
	SET string=!string:sreg=A!
	SET string=!string:m8=A!
	SET string=!string:m16=A!
	SET string=!string:m32=A!
	SET string=!string:m64=A!

	rem type E arguments = 1 byte
	SET string=!string:invalid=- E!

	SET string=!string:	=- !

	echo:!string!>>outputTable.txt

	SET fmtdTableLine=
	SET /A "ignore=0"
	SET /A "ExpandedOpcode=0"
	SET /A "MultiOpcode=0"

	call :processLine
)
goto :eof

:printToFile
rem echo:!ExpandedOpcode!
	IF !ignore! EQU 0 (
		IF !ExpandedOpcode! EQU 0 (
			FOR /L %%D IN (0, 1, !registerOpcode!) do (
				SET /A "perLine=prevNum%%16"
				<NUL SET /P "print=!fmtdTableLine!!size!,	">>outputProcessedTable.txt
				<NUL SET /P "print=!instructionType!,	">>outputProcessedTableInstruction.txt
	
				IF !perLine! EQU 15 (
					echo:>>outputProcessedTable.txt
					echo:>>outputProcessedTableInstruction.txt
				)
				SET /A "prevNum+=1"
			)
		)
		IF !MultiOpcode! EQU 1 (
			SET /A "temp=!prevNum!-1"
			IF NOT !MultiOpcodeIndex! EQU !prevNum! (
				CD.>Instruction_!temp!_subOpcodeInstructionFlag.txt
				SET /A "MultiOpcodeIndex=!prevNum!"
			)
			rem echo:!instructionType!, >>Instruction_!temp!_subOpcodeInstructionFlag.txt
			<NUL SET /P "print=!instructionTypeMulti!,	">>Instruction_!temp!_subOpcodeInstructionFlag.txt
		)
	)
	SET fmtdTableLine=
	SET /A "ignore=0"
	SET /A "ExpandedOpcode=0"
	SET /A "MultiOpcode=0"
goto :eof

:processLine
SET /A "functionIsPrefix=0"
SET /A "registerOpcode=0"
SET /A "argument=0"
SET /A "size=1"
FOR %%B IN (!string!) DO (
	SET "term=%%B"
	IF !argument! EQU 0 (
		IF NOT %%B==- (
			IF NOT "!term:~3!"=="" (
				echo:X	Invalid Size:	%%B	detail: "!term:~4!"
				goto :eof
			)
			SET "testIfHex=" & for /f "delims=0123456789ABCDEF-" %%C in ("%%B") do SET "testIfHex=%%C"
			IF DEFINED testIfHex (
				echo: X	Invalid char:	%%B	detail:	!testIfHex!
				goto :eof
			)

			SET /A "curNum=0x!term:~0,-1!"
			SET /A "functionIsPrefix=1"
			rem SET /A "invalidOpcodeNum=!curNum!-!prevNum!"
			rem FOR /L %%D IN (1, 1, !invalidOpcodeNum!) do (
			rem	SET "instructionType=INSTRUCTION_INVALID"
			rem	SET "fmtdTableLine=OPCODE_INVALID | "
			rem	call :printToFile
			rem )
			IF NOT !prevNum! EQU !curNum! (
				call :indexError %%B
			)
			SET instructionType=INSTRUCTION_INVALID
		)
	)
	IF !argument! EQU 2 (
		IF !functionIsPrefix! EQU 1 IF "!term!" == "-" (
			SET "fmtdTableLine=!fmtdTableLine!OPCODE_PREFIX | "
			goto skip
		) ELSE goto :eof
		IF !term:~3!==r- (
			SET "curNum=0x!term:~0,-3!"
			SET /A "registerOpcode=7"
		) ELSE (
			SET "curNum=0x!term:~0,-1!"
		)
		SET /A "repeatedOpcode=0"
		SET /A "invalidOpcodeNum=!curNum!-!prevNum!"
		FOR /L %%D IN (1, 1, !invalidOpcodeNum!) do (
			SET "instructionType=INSTRUCTION_INVALID"
			SET "fmtdTableLine=OPCODE_INVALID | "
			call :printToFile
		)
		IF NOT !prevNum! EQU !curNum! (
			call :indexError %%B
			SET /A "repeatedOpcode=1"
		)
		SET "fmtdTableLine=!fmtdTableLine!"
	)
	IF !argument! EQU 4 (
		IF NOT %%B==- (
			IF NOT %%B==r- (
				IF !ignore! EQU 1 (
					SET "term=!term:0=z!"
				)
					
				SET "testIfHex=" & for /f "delims=01234567-" %%C in ("!term!") do SET "testIfHex=%%C"
				IF NOT DEFINED testIfHex (
					SET /A "ExpandedOpcode=!term:~0,-1!"
					IF !repeatedOpcode! EQU 1 IF !ExpandedOpcode! EQU !tempExpandedOpcode! (
						call :indexError %%B
						goto :eof
					)
					SET /A "MultiOpcode=1"
					SET /A "tempExpandedOpcode=!ExpandedOpcode!"
					SET /A "ignore=0"
				) ELSE IF !ignore! EQU 1 (
					goto :eof
				)
			)
		)
	)
	IF !argument! EQU 10 (
		IF /I NOT "!term:CALL=!"=="!term!" (
			SET instructionType=INSTRUCTION_CALL
		) ELSE IF /I NOT "!term:JMP=!"=="!term!" (
			SET instructionType=INSTRUCTION_JMP
		) ELSE IF /I NOT "!term:J=!"=="!term!" (
			SET instructionType=INSTRUCTION_CONDITIONAL_JMP
		) ELSE IF /I NOT "!term:RET=!"=="!term!" (
			SET instructionType=INSTRUCTION_RET
		) ELSE (
			SET instructionType=INSTRUCTION_UNKNOW
		)
		SET instructionTypeMulti=!instructionType!
		IF !MultiOpcode! EQU 1 (
			SET instructionType=INSTRUCTION_MULTI_OPCODE
		)
	)
	IF !argument! GEQ 11 (
		IF !argument! LEQ 14 (
			IF %%B==A- (
				SET "fmtdTableLine=!fmtdTableLine!OPCODE_SIB | "
			) ELSE IF %%B==B- (
				SET /A "size+=1"
			) ELSE IF %%B==C- (
				SET "fmtdTableLine=!fmtdTableLine!OPCODE_IMM_BIG | "
			) ELSE IF %%B==D- (
				SET /A "size+=2"
			) ELSE IF %%B==E- (
				SET "fmtdTableLine=!fmtdTableLine!OPCODE_INVALID | "
				SET instructionType=INSTRUCTION_INVALID
				goto skip
			)ELSE IF %%B==F- (
				SET "fmtdTableLine=!fmtdTableLine!OPCODE_IMM | "
				goto skip
			)
		)
	)
	rem echo:!argument! %%B !fmtdTableLine!
	SET /A "argument+=1"
	rem pause
)
:skip
call :printToFile
goto :eof

:indexError
echo:    X	error at index %1	detail:	!curNum! !prevNum!
SET /A "ignore=1"
rem goto :skip
goto :eof