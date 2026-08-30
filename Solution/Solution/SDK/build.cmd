
@echo off

set PathToExternals=.\..\Externals
set Run7z=.\Utilities\Compression\7zip\7za.exe
set PathToResultFile=.\..\..\..\CovelliteSDK_vX.Y.Z.0.7z

echo ===================== Archived =====================

%Run7z% a "%PathToResultFile%" ReadMe.txt
%Run7z% a "%PathToResultFile%" Externals.props
call :SetVersionAndAddToArchive install.cmd
call :SetVersionAndAddToArchive Directory.Build.targets

call AlicornSdkPath.auto.cmd

cd %PathToExternals%

echo Archived Covellite++...
%Run7z% a "%PathToResultFile%" -ir!Covellite -xr!*.pdb -xr!*.idb -xr!*.recipe

echo Archived Alicorn...
%Run7z% a "%PathToResultFile%" %AlicornSdkPath%\*

goto :EndOfFile

:SetVersionAndAddToArchive
copy /Y .\%1 %TEMP%\%1
..\..\..\Release\Bin\x64\Version.exe --file=..\..\Version.hpp --replace=%TEMP%\%1 --find=SDK_VXYYZZ --format=SDK_V%%1%%%%2$02d%%3$02d
%Run7z% a "%PathToResultFile%" %TEMP%\%1
exit /B

:EndOfFile
