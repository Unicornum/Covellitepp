
#pragma once
#include <alicorn\std\singleton.hpp>
#include <alicorn\settings.hpp>

namespace covellite
{

namespace core
{

using Section_t = alicorn::modules::settings::Section;
using SectionPtr_t = ::std::unique_ptr<Section_t>;

/**
* \ingroup CovelliteCoreGroup
* \brief
*  Класс входит в проект \ref CovelliteCorePage \n
*  Класс доступа к настройкам фреймворка.
* \details
*  - Использующая фреймворк программа должна определить функцию Make(),
*  создать объект настроек и установить значения параметров по умолчанию.
*  
* \snippet .\Solution\Covellite.Core\Test.Covellite.Core\Test.Settings\Settings_test.cpp SetDefaultValues
*  
* \version
*  1.0.0.0        \n
*  2.0.0.0        \n
* \date
*  07 Ноябрь 2017    \n
*  23 Февраль 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2017 - 2018
*/
using Settings_t = ::alicorn::extension::std::Singleton<const Section_t &>;

} // namespace core

} // namespace covellite

namespace alicorn
{

namespace extension
{

namespace std
{

using namespace ::covellite::core;

template<>
/*static*/ SectionPtr_t Singleton<Section_t>::Make(void);

} // namespace std

} // namespace extension

} // namespace alicorn
