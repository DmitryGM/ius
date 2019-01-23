@echo off
set path=

set CDIR=d:\c51
set UTILS=d:\UTILS

set c51inc=%CDIR%\inc
set c51lib=%CDIR%\lib

set path=%WINDIR%\COMMAND;%CDIR%\bin;%UTILS%

call clean.bat

cd src

for %%f in (*.c) do C51 %%f   CODE LARGE WL(0) DEBUG OE NOINTVECTOR >> ..\compile.msg
if errorlevel 1 goto ERROR
for %%f in (*.a51) do A51 %%f >> ..\compile.msg
if errorlevel 1 goto ERROR

move /Y startup.obj ..\bin > NUL

echo startup.obj > ..\bin\command.lk
for %%f in (*.obj) do echo ,%%f >> ..\bin\command.lk
echo to example CODE(2100H) XDATA(04000H) >> ..\bin\command.lk

move /Y *.lst ..\lst > NUL
move /Y *.obj ..\bin > NUL
cd ..\bin

BL51 @command.lk > ..\link.msg

OH51 example
if errorlevel 1 goto ERROR

if exist example.hex t167b 0x2100 0x0 addhexstart example.hex bye
goto OK


:ERROR
echo ------------- ERROR ----------------


:OK

cd ..
