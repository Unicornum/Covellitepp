
#include "stdafx.h"
#include <Covellite\Gui\StringTranslator.hpp>

using namespace covellite::gui;

/**
* \brief
*  Функция установки набора строк для преобразования.
* \details
*  - При загрузке .rml файла строки (такие как идентификаторы, 
*  атрибуты и значения xml узлов) будут заменены в соответствии с указанным 
*  набором строк.
*  - Замена строк производится целиком (т.е. подстроки заменятся не будут),
*  поэтому в случае необходимости нужную подстроку следует завернуть,
*  например, в теги \b p (т.е. в строке 'нажмите кнопку \<p\>[BACK]\</p\>.'
*  теперь можно [BACK] заменить на разный текст - например - в зависимости 
*  от платформы).
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
