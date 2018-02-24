@echo off

rem ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
rem Параметры сборки решения
rem ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

set MSBuild="%ProgramFiles% (x86)\MSBuild\14.0\Bin\msbuild.exe"
set Clean=/t:Solution\Build\Clean:Rebuild
set BuildAll=/t:Solution\Build\BuildAll:Rebuild
set Android=/t:Solution\Build\Android:Rebuild

rem ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
rem Признак того, что сборка производится из скрипта
rem ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

set USING_SCRIPT_BILDING=TRUE

rem ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
rem Сборка
rem ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

set Config1=/p:Configuration=Debug.Product;Platform=x64
set Config2=/p:Configuration=Release.Product;Platform=x64
set Config3=/p:Configuration=Debug;Platform=ARM
set Config4=/p:Configuration=Release;Platform=ARM

for %%C in ("%Config1%", "%Config2%", "%Config3%", "%Config4%") do (

for %%S in (*.sln) do (

%MSBuild% %%S %Clean% %%C
%MSBuild% %%S %Prebuild% %%C
%MSBuild% %%S %BuildAll% %%C

)

)

pause
