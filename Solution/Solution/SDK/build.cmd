
@echo off

set PathToExternals=.\..\Externals
set Run7z=.\Utilities\Compression\7zip\7za.exe
set PathToResultFile=.\..\..\..\CovelliteSDK_vX.Y.Z.0.7z

call AlicornSdkPath.auto.cmd

cd %PathToExternals%

echo Archived Covellite++...
%Run7z% a "%PathToResultFile%" -ir!Covellite -xr!*.pdb -xr!*.idb -xr!*.recipe

echo Archived Alicorn...
%Run7z% a "%PathToResultFile%" %AlicornSdkPath%\*
