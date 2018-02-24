@echo off

rem ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
rem Параметры сборки решения
rem ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

set MSBuild="%ProgramFiles% (x86)\MSBuild\14.0\Bin\msbuild.exe"
set Clean=/t:Solution\Build\Clean:Rebuild
set Prebuild=/t:Solution\Build\Prebuild:Rebuild
set BuildAll=/t:Solution\Build\BuildAll:Rebuild

set MSBuildResult=".\Solution\Solution\(Documentation)\MSBuild.result.txt"
set LogInfo=/fileLogger /flp:Append;Verbosity=quiet;LogFile=%MSBuildResult%

rem ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
rem Признак того, что сборка производится из скрипта
rem ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

set USING_SCRIPT_BILDING=TRUE

rem ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
rem Сборка
rem ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

del %MSBuildResult%

set Config1=/p:Configuration=Debug;Platform=x64
set Config2=/p:Configuration=Release;Platform=x64

for %%C in ("%Config1%", "%Config2%") do (

for %%S in (*.sln) do (

%MSBuild% %%S %Clean% %%C
%MSBuild% %%S %Prebuild% %%C
%MSBuild% %%S %BuildAll% %%C %LogInfo% /p:RunCodeAnalysis=true /p:DebugType=none;DebugSymbols=false

)

)

pause
