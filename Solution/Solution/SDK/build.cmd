
@echo off

set PathToExternals=.\..\Externals
set Run7z=.\Utilities\Compression\7zip\7za.exe
set PathToResultFile=.\..\..\..\CovelliteSDK_v0.0.0.0.7z
set PathToBackupPropsFile=.\..\CovellitePropsBackup.7z

cd %PathToExternals%

echo Archived...
%Run7z% a "%PathToResultFile%" -ir!* -x!*.* -x!LICENSE -x!Android -x!Utilities -x!Tools -xr!*.pdb -xr!*.idb -xr!*.recipe
%Run7z% a "%PathToResultFile%" Externals.props

%Run7z% a "%PathToBackupPropsFile%" -ir!Covellite\*.props
