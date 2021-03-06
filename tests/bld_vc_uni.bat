@echo off

@set PATH=C:\softools\mssdk\VC\Bin\x64\;C:\Softools\msvc10\Common7\IDE;%PATH%
@set INCLUDE=C:\softools\mssdk\VC\INCLUDE\;C:\Softools\mssdk\include
@set LIB=..\lib\vc32;C:\Softools\mssdk\VC\lib\x64;C:\Softools\mssdk\lib\x64
@set LIBPATH=..\lib\vc32;C:\Softools\mssdk\VC\lib\x64;C:\Softools\mssdk\lib\x64
rem @set PATH=C:\Softools\msvc10\VC\bin;C:\Softools\msvc10\Common7\IDE;C:\Softools\mssdk\bin;%PATH%
rem @set INCLUDE=C:\Softools\msvc10\VC\include;C:\Softools\mssdk\include
rem @set LIB=..\lib\vc32;C:\Softools\msvc10\VC\lib;C:\Softools\mssdk\lib\x64
rem @set LIBPATH=..\lib\vc32;C:\Softools\msvc10\VC\lib;C:\Softools\mssdk\lib\x64

set BRIG_DIR=\papps\brig
set BRIG_LIBS=brig.lib

cl /c /TP /W3 /nologo /Fo /EHsc -I%BRIG_DIR%\include -DUNICODE %1.cpp >err.out

rem echo 1 24 "..\include\WindowsXP.Manifest" > hwgui_xp.rc
rem brc32 -r hwgui_xp -fohwgui_xp

link -SUBSYSTEM:WINDOWS %1.obj %BRIG_LIBS% user32.lib gdi32.lib comdlg32.lib shell32.lib comctl32.lib winspool.lib OleAut32.Lib Ole32.Lib >>err.out
mt.exe -manifest ..\include\WindowsXP.Manifest -outputresource:%1.exe;1

del *.obj
