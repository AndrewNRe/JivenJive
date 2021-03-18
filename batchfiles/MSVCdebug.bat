taskkill /IM devenv.exe /F
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x86
cd c:\JivenJive
devenv /Command "Debug.SymbolPath C:\JivenJive\src" /Command "Debug.SymbolPath C:\JivenJive\build" c:\JivenJive\build\JivenJive_Windows.exe