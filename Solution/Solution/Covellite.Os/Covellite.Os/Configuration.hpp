
#pragma once
#include <alicorn\std\string.forward.hpp>

namespace covellite
{

namespace os
{

/**
* \ingroup CovelliteOsGroup
* \brief
*  Класс входит в проект \ref CovelliteOsPage \n
*  Класс доступа к текущим настройкам системы.
*
* \version
*  1.0.0.0        \n
*  1.1.0.0        \n
* \date
*  27 Май 2017    \n
*  24 Октябрь 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2017
*/
class Configuration final
{
  using String_t = ::alicorn::extension::std::String;

public:
  /**
  * \brief
  *  Функция получения текущего системного языка.
  * \details
  *  - Назначение функции - получение строки, которую можно непосредственно
  *  использовать для построения имени файла, содержащего локализованные
  *  строки, используемые программой.
  *
  * \return "English"
  *  Для английского языка.
  * \return "Russian"
  *  Для русского языка.
  * \return "Default"
  *  Если определить язык системы не удалось.
  */
  String_t GetSystemLanguage(void) const;

private:
  String_t GetRawSystemLanguage(void) const;
};

} // namespace os

} // namespace covellite
