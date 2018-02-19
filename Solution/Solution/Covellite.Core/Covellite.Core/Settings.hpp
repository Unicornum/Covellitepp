
#pragma once
#include <alicorn\settings.hpp>

namespace covellite
{

namespace core
{

/**
* \ingroup gCovelliteCoreClasses
* \brief
*  Класс входит в проект \ref CovelliteCorePage \n
*  Класс доступа к настройкам фреймворка.
* \details
*  - Использующая фреймворк программа должна определить функцию
*  SetDefaultValues() и  установить значения параметров по умолчанию.
* \code
using namespace covellite::core;
  
void Settings::SetDefaultValues(Section_t & _Covellitepp)
{
  _Covellitepp.SetDefault(uT("Name"), uT("Value"), uT("Description"));
}
* \endcode
*  
* \version
*  1.0.0.0        \n
* \date
*  07 Ноябрь 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2017
*/
class Settings final
{
  using Section_t = alicorn::modules::settings::Section;

public:
  static const Settings & GetInstance(void);
  Section_t GetFrameworkSection(void) const;

private:
  static void SetDefaultValues(Section_t &);

private:
  Settings(void) = default;
};

} // namespace core

} // namespace covellite
