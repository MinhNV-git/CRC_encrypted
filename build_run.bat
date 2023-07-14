@echo off
CLS
:start
gcc main.c crc.c -o run && run
pause
goto start