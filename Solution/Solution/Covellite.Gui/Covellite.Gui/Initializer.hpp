
// Так нужно - для тестов.
#ifndef COVELLITE_GUI_INITIALIZER_HPP
#define COVELLITE_GUI_INITIALIZER_HPP
#include <Covellite\Gui\File.hpp>
#include <Covellite\Gui\System.hpp>
#include <memory>

namespace covellite
{

namespace gui
{

class IStringTranslator;

/**
* \ingroup CovelliteGuiGroup
* \brief
*  Класс входит в проект \ref CovelliteGuiPage \n
*  Класс, инкапсулирующий инициализацию/разрушение используемой библиотеки GUI.
* \todo
*  Класс используется для внутренней реализации и не должен попадать в Externals.
*
* \version
*  1.0.0.0        \n
*  1.1.0.0        \n
*  1.2.0.0        \n
*  1.3.0.0        \n
* \date
*  19 Декабрь 2016    \n
*  26 Декабрь 2016    \n
*  29 Сентябрь 2017    \n
*  08 Октябрь 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2016 - 2017
*/
class Initializer final
{
  using RenderInterfacePtr_t = 
    ::std::shared_ptr<CovelliteGui::RenderInterface>;
  using StringTranslatorPtr_t = 
    ::std::shared_ptr<covellite::gui::IStringTranslator>;
  class Helper;

public:
  class Data
  {
  public:
    RenderInterfacePtr_t  m_pRenderInterface;
    StringTranslatorPtr_t m_pStringTranslator;
  };

private:
  covellite::gui::File   m_File;
  covellite::gui::System m_System;
  RenderInterfacePtr_t   m_pRenderInterface;

public:
  explicit Initializer(const Data &);
  ~Initializer(void);
};

} // namespace gui

} // namespace covellite

#endif // COVELLITE_GUI_INITIALIZER_HPP
