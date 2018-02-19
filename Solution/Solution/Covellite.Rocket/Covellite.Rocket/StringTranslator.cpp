
#include "stdafx.h"
#include <Covellite\Rocket\StringTranslator.hpp>

using namespace covellite::rocket;

/**
* \brief
*  Функция установки набора строк для преобразования.
*  
* \param [in] _Strings
*  Набор строк, в котром первое значение - строка, поиск которой будет 
*  производиться при вызове функции Translate(), вторая - значение, кторое
*  будет возвращено функцией Translate().
*/
void StringTranslator::Set(const Bank_t & _Strings)
{
  m_Strings = _Strings;
}

auto StringTranslator::Translate(const Utf8String_t & _Source) ->Utf8String_t /*override*/
{
  auto itSource = m_Strings.find(_Source);
  if (itSource == m_Strings.end()) return _Source;

  return itSource->second;
}
