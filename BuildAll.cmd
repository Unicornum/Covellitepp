@echo off

set PathToExternals=.\Solution\Solution\Externals

FOR /F "usebackq delims=:/ tokens=1,3*" %%j IN (`git rev-parse --abbrev-ref HEAD`) DO (
if "%%j"=="develop" goto DoDevelop
if "%%j"=="master" goto DoMaster
echo.
echo Unexpected branch: %%j
echo Valid branch:
echo   trunk or release - Debug and Release build configurations
echo   master or tag - Debug.Product and Release.Product build configurations
)

goto End

rem ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
:DoDevelop

set BuildAll=BuildAll:Rebuild

call %PathToExternals%\BuildSolution.cmd "Release:x64,Debug:x64" "Clean:Rebuild,Prebuild:Rebuild"

rem ARM64 собирается раньше x64, чтобы результат сборки попадал в документацию.
call %PathToExternals%\BuildSolution.cmd "Release:ARM64,Debug:ARM64" "%BuildAll%"

call %PathToExternals%\BuildSolution.cmd "Release:x64,Debug:x64" "%BuildAll%"
call %PathToExternals%\BuildSolution.cmd "Release:ARM,Debug:ARM" "%BuildAll%"

goto End

rem ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
:DoMaster

call %PathToExternals%\BuildSolution.cmd "Debug.Product:x64,Release.Product:x64" "Clean:Rebuild,Prebuild:Rebuild,BuildAll:Rebuild"
call %PathToExternals%\BuildSolution.cmd "Debug.Product:ARM,Release.Product:ARM" "Prebuild:Rebuild,BuildAll:Rebuild"
rename .\Release\Example.apk Covellitepp_ARM.apk

call %PathToExternals%\BuildSolution.cmd "Debug.Product:ARM64,Release.Product:ARM64" "Prebuild:Rebuild,BuildAll:Rebuild"
rename .\Release\Example.apk Covellitepp_ARM64.apk

cd .\Solution\Solution\SDK
call build.cmd

goto End

rem ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
:End

pause
