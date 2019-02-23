
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
