
/// \file
///  Файл содержит список строк, которые должен игнорировать CppCheck. \n
///  Для .cpp файлов лучше использовать inline suppression - предварительный
///  комментарий вида \n 
///  // cppcheck-suppress suppress_id \n
///  Формат записей: 'suppress_id:File:Line'.

//*

missingIncludeSystem:*
missingInclude:*

unusedFunction:*\Mock\*.hpp
unusedFunction:*.mock.*
unusedFunction:*\Renderer\GLExt.hpp
unusedFunction:*\Platform\Event.hpp
