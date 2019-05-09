
#pragma once
#include <Rocket\Rocket.hpp>
#include <Covellite\Gui\IStringTranslator.hpp>

namespace std { template<class> class shared_ptr; }

namespace covellite
{

namespace gui
{

/**
* \ingroup CovelliteGuiGroup
* \brief
*  Класс входит в проект \ref CovelliteGuiPage \n
*  Класс реализации системного интерфейса libRocket.
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
  public Rocket::Core::SystemInterface
{
  using String_t = Rocket::Core::String;
  using StringTranslatorPtr_t = ::std::shared_ptr<IStringTranslator>;

public:
  float GetElapsedTime(void) override;
  int TranslateString(String_t &, const String_t &) override;
  bool LogMessage(Rocket::Core::Log::Type, const String_t &) override;
  void ActivateKeyboard(void) override;
  void DeactivateKeyboard(void) override;

private:
  StringTranslatorPtr_t m_pStringTranslator;

public:
  explicit System(const StringTranslatorPtr_t &);
};

} // namespace gui

} // namespace covellite
