
@echo off

rem TODO: ��� ������� �� ������� ����� ������ ����� ���������� ���� ��� ��������
set PathToExternals=.\..\Externals
set Run7z=%PathToExternals%\Utilities\Compression\7zip\7z.exe
set PathToResultFile=.\..\..\..\CovelliteSDK_v0.0.0.0.7z

set Options=-t7z -r -mhe

rem TODO: ������-�� ����� �� ����������� �����, ��������� ���
rem Externals\*.*, � ���� ������� *.*, �� ����������� ��� ����� ����������... 
set Exclude=-scsWIN -x@exclude.txt

echo Archived...
%Run7z% a %Options% %Exclude% "%PathToResultFile%" "%PathToExternals%\*"

rem ... ������� ������� ������ ����� �� ������ ��������
%Run7z% d "%PathToResultFile%" *.*

pause
