
#pragma once
#include <Covellite\Gui\IStringTranslator.hpp>
#include <memory>

namespace covellite
{

namespace gui
{

/**
* \ingroup CovelliteGuiGroup
* \brief
*  Класс входит в проект \ref CovelliteGuiPage \n
*  Класс реализации системного интерфейса.
* \todo
*  Класс используется для внутренней реализации и не должен попадать в Externals.
*
* \version
*  1.0.0.0        \n
*  1.1.0.0        \n
*  1.2.0.0        \n
* \date
*  07 Декабрь 2016    \n
*  19 Декабрь 2016    \n
*  08 Октябрь 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2016 - 2017
*/
class System final :
  public CovelliteGui::SystemInterface
{
  using String_t = CovelliteGui::String;
  using StringTranslatorPtr_t = ::std::shared_ptr<IStringTranslator>;

public:
  CovelliteGuiTime_t GetElapsedTime(void) override;
  int TranslateString(String_t &, const String_t &) override;
  bool LogMessage(CovelliteGui::Log::Type, const String_t &) override;
  void ActivateKeyboard(Rml::Vector2f caret_position, float line_height) override;
  void DeactivateKeyboard(void) override;

private:
  StringTranslatorPtr_t m_pStringTranslator;

public:
  explicit System(const StringTranslatorPtr_t &);
};

} // namespace gui

} // namespace covellite
