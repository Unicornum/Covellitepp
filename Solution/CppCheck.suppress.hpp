
/// \file
///  Файл содержит список строк, которые должен игнорировать CppCheck. \n
///  Для .cpp файлов лучше использовать inline suppression - предварительный
///  комментарий вида \n 
///  // cppcheck-suppress ErrorId \n
///  Формат записей: '[ErrorId]:[File]:[Line]'.

//*

syntaxError:*\*test.cpp
unusedFunction:*\*_test.cpp
shadowFunction:*\Test.Covellite.Os\Test.Window\Window.android_test.cpp
functionStatic:*\Test.Covellite.Gui\Mock\*.hpp

passedByValue:*\shaders\*.fx
pureVirtualCall:*\Covellite.Api\Renderer\GraphicApi.hpp
virtualCallInConstructor:*\Covellite.Api\Renderer\GraphicApi.hpp
constParameter:*\Covellite.Os\Window.android.cpp
