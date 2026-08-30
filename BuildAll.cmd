@echo off

set PathToExternals=.\Solution\Solution\Externals

FOR /F "usebackq delims=:/ tokens=1,3*" %%j IN (`git rev-parse --abbrev-ref HEAD`) DO (
if "%%j"=="develop" goto DoDevelop
if "%%j"=="master" goto DoMaster
echo.
echo Unexpected branch: %%j
echo Valid branch:
echo   develop - Debug and Release build configurations
echo   master - Debug.Product and Release.Product build configurations
)

goto End

rem ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
:DoDevelop

set BuildAll=BuildAll:Rebuild

call %PathToExternals%\BuildSolution.cmd "Release:x64,Debug:x64" "Clean:Rebuild,Prebuild:Rebuild"

rem ARM64 собирается раньше x64, чтобы результат сборки попадал в документацию.
call %PathToExternals%\BuildSolution.cmd "Release:ARM64,Debug:ARM64" "%BuildAll%"
call %PathToExternals%\BuildSolution.cmd "Release:x64,Debug:x64" "%BuildAll%"

goto End

rem ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
:DoMaster

call %PathToExternals%\BuildSolution.cmd "Debug.Product:x64,Release.Product:x64" "Clean:Rebuild,Prebuild:Rebuild,BuildAll:Rebuild"
call %PathToExternals%\BuildSolution.cmd "Debug.Product:ARM64,Release.Product:ARM64" "Prebuild:Rebuild,BuildAll:Rebuild"
rename .\Release\Example.apk Covellitepp_ARM64.apk

copy /Y .\Solution\Solution\SDK\build.cmd .\Solution\Solution\SDK\build_version.cmd
.\Release\Bin\x64\Version.exe --file=.\Solution\Version.hpp --replace=.\Solution\Solution\SDK\build_version.cmd --find=SDK_vX.Y.Z --format="SDK_v%%1%%.%%2%%.%%3%%"

cd .\Solution\Solution\SDK
call build.cmd

goto End

rem ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
:End

pause
