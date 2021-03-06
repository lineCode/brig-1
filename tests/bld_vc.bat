@echo off

@set PATH=C:\Softools\msvc10\VC\bin;C:\Softools\msvc10\Common7\IDE;C:\Softools\mssdk\bin;%PATH%
@set INCLUDE=C:\Softools\msvc10\VC\include;C:\Softools\mssdk\include
@set LIB=..\lib\vc32;C:\Softools\msvc10\VC\lib;C:\Softools\mssdk\lib
@set LIBPATH=..\lib\vc32;C:\Softools\msvc10\VC\lib;C:\Softools\mssdk\lib

set BRIG_DIR=\papps\brig
set BRIG_LIBS=brig_ansi.lib

cl /c /TP /W3 /nologo /Fo -I%BRIG_DIR%\include %1.cpp >err.out

rem echo 1 24 "..\include\WindowsXP.Manifest" > hwgui_xp.rc
rem brc32 -r hwgui_xp -fohwgui_xp

link -SUBSYSTEM:WINDOWS %1.obj %BRIG_LIBS% user32.lib gdi32.lib comdlg32.lib shell32.lib comctl32.lib winspool.lib OleAut32.Lib Ole32.Lib >>err.out
mt.exe -manifest ..\include\WindowsXP.Manifest -outputresource:%1.exe;1

del *.obj
