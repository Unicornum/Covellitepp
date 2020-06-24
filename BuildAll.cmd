@echo off

set PathToExternals=.\Solution\Solution\Externals

FOR /F "usebackq delims=:/ tokens=1,3*" %%i IN (`svn info`) DO (
if "%%i"=="Relative URL" (
if "%%j"=="trunk" goto DoRelease
if "%%j"=="release" goto DoRelease
if "%%j"=="tag" goto DoMaster
if "%%j"=="master" goto DoMaster
echo Unexpected branch: %%j
echo Valid branch:
echo   trunk or release - Debug and Release build configurations
echo   master or tag - Debug.Product and Release.Product build configurations
)
)

goto End

rem ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
:DoRelease

set BuildAll=BuildAll:Rebuild

call %PathToExternals%\BuildSolution.cmd "Release:x64,Debug:x64" "Clean:Rebuild,Prebuild:Rebuild,%BuildAll%"

goto End

rem ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
:DoMaster

call %PathToExternals%\BuildSolution.cmd "Debug.Product:x64,Release.Product:x64" "Clean:Rebuild,Prebuild:Rebuild,BuildAll:Rebuild"
call %PathToExternals%\BuildSolution.cmd "Release.Product:ARM" "Prebuild:Rebuild,BuildAll:Rebuild"

rename .\Release\Example.apk Covellitepp.apk

goto End

rem ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
:End

pause
