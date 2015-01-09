@ECHO OFF
REM run from source root: test/test.bat
set PATH=../bin;%PATH%
lua test.lua
