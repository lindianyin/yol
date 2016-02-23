@call :startlog
@timeout /t 1 /nobreak >nul
@call :startccr
@timeout /t 5 /nobreak >nul
@call :startcr
@timeout /t 3 /nobreak >nul
@call :startgame gs1
rem @call :startgame gs2
rem @call :startgame gs3
rem @call :startgame gs4
rem @call :startgame cgs1
rem @call :startgame cgs2

@call :startgate
@goto:eof


:startlog
	@xcopy  "..\Debug\LogServer.*"      ".\log\" /y /d 1>nul
	@xcopy  "..\Debug\libmysql.dll"     ".\log\" /y /d 1>nul
	@cd log
	@start LogServer
	@cd ..
	@goto:eof

:startccr
	@xcopy  "..\Debug\coordinator2.*"   ".\ccr\" /y /d 1>nul
	@xcopy  "..\Debug\libmysql.dll"     ".\ccr\" /y /d 1>nul
	@mklink ".\ccr\script"              "..\..\..\script"   /D 2>nul
	@mklink ".\ccr\setting"             "..\..\..\setting"  /D 2>nul
	@mklink ".\ccr\scene"               "..\..\..\scene"    /D 2>nul
	@cd ccr
	@start coordinator2
	@cd ..
	@goto:eof

:startcr
	@xcopy  "..\Debug\coordinator2.*"   ".\cr\" /y /d 1>nul
	@xcopy  "..\Debug\libmysql.dll"     ".\cr\" /y /d 1>nul
	@mklink ".\cr\script"               "..\..\..\script"   /D 2>nul
	@mklink ".\cr\setting"              "..\..\..\setting"  /D 2>nul
	@mklink ".\cr\scene"                "..\..\..\scene"    /D 2>nul
	@cd cr
	@start coordinator2
	@cd ..
	@goto:eof

:startgate
	@xcopy  "..\Debug\gateway.*"        ".\gate\" /y /d 1>nul
	@cd gate
	@start gateway
	@cd ..
	@goto:eof

:startgame
	@xcopy  "..\Debug\gameserver.*"     ".\%1" /y/d 1>nul
	@mklink ".\%1\script"             "..\..\..\script"   /D 2>nul
	@mklink ".\%1\setting"            "..\..\..\setting"  /D 2>nul
	@mklink ".\%1\scene"              "..\..\..\scene"    /D 2>nul
	@cd %1
	@start gameserver
	@cd ..
	@goto:eof

