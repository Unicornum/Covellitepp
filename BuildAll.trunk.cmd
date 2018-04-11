@echo off

set PathToExternals=.\Solution\Solution\Externals
set BuildAll=BuildAll:Rebuild /p:RunCodeAnalysis=true

call %PathToExternals%\MsBuild.auto.cmd
call %PathToExternals%\BuildSolution.cmd "Debug:x64,Release:x64" "Clean:Rebuild,Prebuild:Rebuild,%BuildAll%"

pause
