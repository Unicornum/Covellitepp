
/// \file
///  Файл содержит список строк, которые должен игнорировать CppCheck. \n
///  Для .cpp файлов лучше использовать inline suppression - предварительный
///  комментарий вида \n 
///  // cppcheck-suppress ErrorId \n
///  Формат записей: '[ErrorId]:[File]:[Line]'.

//*

memsetClassFloat:*\Renderer\DirectX*
syntaxError:*\*test.cpp
unusedFunction:*\*_test.cpp
functionStatic:*\Covellite.Gui\Test.Covellite.Gui\Mock\*.hpp
functionConst:*\Externals.doxygen.ignore\Assimp\*
functionStatic:*\Externals.doxygen.ignore\Assimp\*
noExplicitConstructor:*\Externals.doxygen.ignore\Assimp\*
variableScope:*\Externals.doxygen.ignore\Assimp\*
ConfigurationNotChecked:*\Externals.doxygen.ignore\Assimp\*
