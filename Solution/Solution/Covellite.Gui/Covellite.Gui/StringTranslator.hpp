
#pragma once
#include <Covellite\Gui\IStringTranslator.hpp>
#include <map>

namespace covellite
{

namespace gui
{

/**
* \ingroup CovelliteGuiGroup
* \brief
*  Класс входит в проект \ref CovelliteGuiPage \n
*  Класс преобразователя строк.
* \details
*  - При создании объекта covellite::gui::Window ему следует передать
*  набор строк, который будет использоваться для преобразования.
* \todo
*  Класс используется для внутренней реализации и не должен попадать в Externals.
*  
* \version
*  1.0.0.0        \n
* \date
*  06 Октябрь 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2017
*/
class StringTranslator final :
  public IStringTranslator
{
  using Bank_t = ::std::map<Utf8String_t, Utf8String_t>;

public:
  void Set(const Bank_t &);

public:
  // Интерфейс IStringTranslator:
  Utf8String_t Translate(const Utf8String_t &) override;

private:
  Bank_t m_Strings;
};

} // namespace gui

} // namespace covellite
