@echo off

set PathToExternals=.\Solution\Solution\Externals

call %PathToExternals%\MsBuild.auto.cmd
call %PathToExternals%\BuildSolution.cmd "Debug.Product:x64,Release.Product:x64" "Clean:Rebuild,Prebuild:Rebuild,BuildAll:Rebuild"

rename .\Release\Example.apk Covellitepp.apk

pause
