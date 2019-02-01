
#pragma once

namespace covellite
{

namespace gui
{

/**
* \ingroup CovelliteGuiGroup
* \brief
*  Класс входит в проект \ref CovelliteGuiPage \n
*  Интерфейсный класс преобразователя строк libRocket.
*  
* \version
*  1.0.0.0        \n
* \date
*  21 Декабрь 2016    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2016
*/
class IStringTranslator
{
protected:
  using Utf8String_t = ::std::string;

public:
  /// Функция преобразования 'сырой' строки (прочитанной из .rml файла)
  /// в строку, которая будет отображаться на экране (преобразование 
  /// предназначено, в первую очередь, для локализации приложений).
  virtual Utf8String_t Translate(const Utf8String_t &) = 0;

public:
  virtual ~IStringTranslator(void) = default;
};

} // namespace gui

} // namespace covellite
