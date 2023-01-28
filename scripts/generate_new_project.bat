@echo off
rem voir https://stackoverflow.com/questions/5034076/what-does-dp0-mean-and-how-does-it-work
rem le nom du script powershell doit être le même que celui du batch, mais précédé d'un underscore
set PSScript=%~dp0_%~nn0.ps1
set args=%1
:More
shift
if '%1'=='' goto Done
set args=%args%, %1
goto More
:Done
powershell.exe -ExecutionPolicy Bypass -Command "& '%PSScript%' '%args%'"
pause