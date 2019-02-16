@echo off

set PathToExternals=.\Solution\Solution\Externals

call %PathToExternals%\MsBuild.auto.cmd

FOR /F "usebackq delims=:/ tokens=1,3*" %%i IN (`svn info`) DO (
if "%%i"=="Relative URL" (
if "%%j"=="trunk" goto DoRelease
if "%%j"=="release" goto DoRelease
if "%%j"=="tag" goto DoMaster
if "%%j"=="master" goto DoMaster
echo Unexpected branch: %%j
)
)

goto End

rem ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
:DoRelease

set BuildAll=BuildAll:Rebuild /p:RunCodeAnalysis=true /p:EnableCppCoreCheck=true

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
