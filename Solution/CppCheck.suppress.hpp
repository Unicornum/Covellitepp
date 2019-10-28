
/// \file
///  Файл содержит список строк, которые должен игнорировать CppCheck. \n
///  Для .cpp файлов лучше использовать inline suppression - предварительный
///  комментарий вида \n 
///  // cppcheck-suppress ErrorId \n
///  Формат записей: '[ErrorId]:[File]:[Line]'.

//*

syntaxError:*\*test.cpp
unusedFunction:*\*_test.cpp
functionStatic:*\Covellite.Gui\Test.Covellite.Gui\Mock\*.hpp
passedByValue:*\shaders\*.fx

functionConst:*\Externals.doxygen.ignore\Assimp\*
functionStatic:*\Externals.doxygen.ignore\Assimp\*
noExplicitConstructor:*\Externals.doxygen.ignore\Assimp\*
variableScope:*\Externals.doxygen.ignore\Assimp\*
ConfigurationNotChecked:*\Externals.doxygen.ignore\Assimp\*
shiftTooManyBits:*\Externals.doxygen.ignore\Assimp\*
clarifyCalculation:*\Externals.doxygen.ignore\Assimp\*
funcArgNamesDifferent:*\Externals.doxygen.ignore\Assimp\*
unreadVariable:*\Externals.doxygen.ignore\Assimp\*
memleak:*\Externals.doxygen.ignore\Assimp\*
unusedFunction:*\Externals.doxygen.ignore\Assimp\*
