call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
cd c:\JivenJive\
pushd build
del /s JivenJive_Windows.asm
del /s JivenJive_Windows.exe
del /s JivenJive_Windows.obj
del /s JivenJive_Windows.pdb
del /s vc140.pdb
@echo START COMPILATION: %time%
cl /c /utf-8 /Od /Oi /MTd /W1 /FC /Zi /FA /I c:\JivenJive\src /I c:\JivenJive\src\vendor c:\JivenJive\src\JivenJive_Windows.cpp
link /DEBUG:FULL /incremental:no /OPT:REF JivenJive_Windows.obj kernel32.Lib User32.Lib Opengl32.lib Gdi32.lib Mincore.lib hid.lib Winmm.lib Ole32.lib
popd
@echo END COMPILATION: %time%