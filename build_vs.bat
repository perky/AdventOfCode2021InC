SET VS=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools
CALL "%VS%\VsDevCmd.bat" -arch=x64

SET CommonFlags=-nologo -O2 -Oi /Zi /Gm- /Gd /TC
SET CommonLinkerFlags=-opt:ref kernel32.lib
cl.exe %CommonFlags% main.c /link /OUT:"aoc.exe"  %CommonLinkerFlags%