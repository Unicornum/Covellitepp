@echo off

set PathToExternals=.\Solution\Solution\Externals
set BuildAll=BuildAll:Rebuild /p:RunCodeAnalysis=true

call %PathToExternals%\MsBuild.auto.cmd
call %PathToExternals%\BuildSolution.cmd "Release:x64,Debug:x64" "Clean:Rebuild,Prebuild:Rebuild,%BuildAll%"

pause
