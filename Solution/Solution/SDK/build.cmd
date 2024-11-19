
@echo off

rem TODO: для запуска из скрипта общей сборки нужно передавать путь как параметр
set PathToExternals=.\..\Externals
set Run7z=%PathToExternals%\Utilities\Compression\7zip\7z.exe
set PathToResultFile=.\..\..\..\CovelliteSDK_v0.0.0.0.7z

set Options=-t7z -r -mhe

rem TODO: Почему-то здесь не исключаются файлы, указанные как
rem Externals\*.*, а если указать *.*, то исключаются все файлы рекурсивно... 
set Exclude=-scsWIN -x@exclude.txt

echo Archived...
%Run7z% a %Options% %Exclude% "%PathToResultFile%" "%PathToExternals%\*"

rem ... поэтому удаляем лишние файлы из архива отдельно
%Run7z% d "%PathToResultFile%" *.*

pause
